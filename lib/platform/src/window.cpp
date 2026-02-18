#include "tgui/core/base.h"
#include "tgui/core/assert.h"

#include "tgui/platform/window.h"
#include "tgui/platform/input.h"

#include "tgui/event/key_events.h"
#include "tgui/event/mouse_events.h"
#include "tgui/event/window_events.h"

#if defined(TGUI_PLATFORM_WINDOWS)
	#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(TGUI_PLATFORM_LINUX)
	#define GLFW_EXPOSE_NATIVE_X11
	#define GLFW_EXPOSE_NATIVE_WAYLAND
#elif defined(TGUI_PLATFORM_MACOS) || defined(TSTL_PLATFORM_IOS)
	#define GLFW_EXPOSE_NATIVE_COCOA
#else
#pragma error("unsupported platform")
#endif

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace tgui::platform
{
	static void glfw_error_callback(int error, const char* description)
	{
		TGUI_ERROR_TAG("GLFW", "{0}: {1}", error, description);
	}

	window::window(const window_adapter::properties& props)
		: window_adapter(props)
	{
		init();
	}

	window::~window()
	{
		shutdown();
	}

	void window::process_events()
	{
		glfwPollEvents();
	}

	void window::set_event_callback(const event_callback_fn& cb)
	{
		m_event_callback = cb;
	}

	void window::on_event(event& ev)
	{
		if (m_event_callback)
			m_event_callback(ev);
	}

	void window::set_mouse_pos(const pixel_position& pos)
	{
		glfwSetCursorPos((GLFWwindow*)m_window, pos.x, pos.y);
	}

	pixel_position window::get_mouse_pos() const
	{
		double x, y;
		glfwGetCursorPos((GLFWwindow*)m_window, &x, &y);
		return { (int32_t)x, (int32_t)y };
	}

	void window::set_size(const pixel_size& size)
	{
		glfwSetWindowSize((GLFWwindow*)m_window, size.width, size.height);
		m_props.size = size;
	}

	pixel_size window::get_size() const
	{
		int w = 0, h = 0;
		glfwGetWindowSize((GLFWwindow*)m_window, &w, &h);
		return { w, h };
	}

	void window::set_position(const pixel_position& pos)
	{
		glfwSetWindowPos((GLFWwindow*)m_window, pos.x, pos.y);
	}

	pixel_position window::get_position() const
	{
		int x = 0, y = 0;
		glfwGetWindowPos((GLFWwindow*)m_window, &x, &y);
		return { x, y };
	}

	void window::set_title(const std::string& title)
	{
		glfwSetWindowTitle((GLFWwindow*)m_window, title.c_str());
		m_props.title = title;
	}

	const std::string& window::get_title() const
	{
		return m_props.title;
	}

	bool window::is_minimized() const
	{
		return glfwGetWindowAttrib((GLFWwindow*)m_window, GLFW_ICONIFIED) == GLFW_TRUE;
	}

	bool window::is_maximized() const
	{
		return glfwGetWindowAttrib((GLFWwindow*)m_window, GLFW_MAXIMIZED) == GLFW_TRUE;
	}

	void window::show()
	{
		glfwShowWindow((GLFWwindow*)m_window);
	}

	void window::hide()
	{
		glfwHideWindow((GLFWwindow*)m_window);
	}

	void* window::get_internal_window() const
	{
		return m_window;
	}

	window::native_window_handle window::get_native_window() const
	{
#if TGUI_PLATFORM_WINDOWS
		return (window::native_window_handle)glfwGetWin32Window((GLFWwindow*)m_window);
#elif TGUI_PLATFORM_LINUX
		return (window::native_window_handle)glfwGetX11Window((GLFWwindow*)m_window);
#elif TGUI_PLATFORM_IOS || TGUI_PLATFORM_MACOS
		return (window::native_window_handle)glfwGetCocoaWindow((GLFWwindow*)m_window);
#else
		return nullptr;
#endif
	}

	void window::init()
	{
		int res = glfwInit();
		if (!res) {
			TGUI_ERROR_TAG("GLFW", "Failed to initialize window");
			return;
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_FALSE);
		glfwWindowHint(GLFW_SCALE_FRAMEBUFFER, GLFW_FALSE);

		if (!m_props.decorated)
			glfwWindowHint(GLFW_DECORATED, false);

		if (m_props.fullscreen) {
			GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

			glfwWindowHint(GLFW_DECORATED, false);
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

			m_window = glfwCreateWindow(mode->width, mode->height, m_props.title.c_str(), primaryMonitor, nullptr);
		}
		else
		{
			m_window = glfwCreateWindow((int)m_props.size.width, (int)m_props.size.height, m_props.title.c_str(), nullptr, nullptr);
		}

		glfwSetWindowUserPointer((GLFWwindow*)m_window, this);

		glfwSetWindowSizeCallback((GLFWwindow*)m_window, [](GLFWwindow* window, int width, int height)
		{
			tgui::platform::window* pWindow = (tgui::platform::window*)glfwGetWindowUserPointer(window);

			window_resize_event event(width, height);
			pWindow->m_props.size.width = width;
			pWindow->m_props.size.height = height;
			pWindow->on_event(event);
		});

		glfwSetWindowPosCallback((GLFWwindow*)m_window, [](GLFWwindow* window, int x, int y)
		{
			tgui::platform::window* pWindow = (tgui::platform::window*)glfwGetWindowUserPointer(window);

			window_move_event event(x, y);
			pWindow->on_event(event);
		});

		glfwSetWindowIconifyCallback((GLFWwindow*)m_window, [](GLFWwindow* window, int iconified)
		{
			tgui::platform::window* pWindow = (tgui::platform::window*)glfwGetWindowUserPointer(window);

			window_minimize_event event(iconified == GLFW_TRUE);
			pWindow->on_event(event);
		});

		glfwSetWindowMaximizeCallback((GLFWwindow*)m_window, [](GLFWwindow* window, int maximized)
		{
			tgui::platform::window* pWindow = (tgui::platform::window*)glfwGetWindowUserPointer(window);

			window_maximize_event event(maximized == GLFW_TRUE);
			pWindow->on_event(event);
		});

		glfwSetWindowCloseCallback((GLFWwindow*)m_window, [](GLFWwindow* window) 
		{
			tgui::platform::window* pWindow = (tgui::platform::window*)glfwGetWindowUserPointer(window);

			window_close_event event;
			pWindow->on_event(event);
		});

		glfwSetKeyCallback((GLFWwindow*)m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) 
		{
			tgui::platform::window* pWindow = (tgui::platform::window*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					input::update_key_state((key_code)key, key_state::pressed);
					key_pressed_event event((key_code)key, 0);
					pWindow->on_event(event);
				} break;
				case GLFW_RELEASE:
				{
					input::update_key_state((key_code)key, key_state::released);
					key_released_event event((key_code)key);
					pWindow->on_event(event);
				} break;
				case GLFW_REPEAT:
				{
					input::update_key_state((key_code)key, key_state::held);
					key_pressed_event event((key_code)key, 1);
					pWindow->on_event(event);
				} break;
			}
		});

		glfwSetCharCallback((GLFWwindow*)m_window, [](GLFWwindow* window, uint32_t codepoint) 
		{
			tgui::platform::window* pWindow = (tgui::platform::window*)glfwGetWindowUserPointer(window);

			key_typed_event event((key_code)codepoint);
			pWindow->on_event(event);
		});

		glfwSetMouseButtonCallback((GLFWwindow*)m_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			tgui::platform::window* pWindow = (tgui::platform::window*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					input::update_mouse_state((mouse_button)button, key_state::pressed);
					mouse_pressed_event event((mouse_button)button);
					pWindow->on_event(event);
					break;
				}
				case GLFW_RELEASE:
				{
					input::update_mouse_state((mouse_button)button, key_state::released);
					mouse_released_event event((mouse_button)button);
					pWindow->on_event(event);
					break;
				}
			}
		});

		glfwSetScrollCallback((GLFWwindow*)m_window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			tgui::platform::window* pWindow = (tgui::platform::window*)glfwGetWindowUserPointer(window);

			mouse_scrolled_event event((float)xOffset, (float)yOffset);
			pWindow->on_event(event);
		});

		glfwSetCursorPosCallback((GLFWwindow*)m_window, [](GLFWwindow* window, double x, double y)
		{
			tgui::platform::window* pWindow = (tgui::platform::window*)glfwGetWindowUserPointer(window);

			mouse_moved_event event((float)x, (float)y);
			pWindow->on_event(event);
		});

		input::init(this);
	}

	void window::shutdown()
	{
		input::shutdown();

		glfwTerminate();
	}
}