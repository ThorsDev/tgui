#include "tgui/core/base.h"
#include "tgui/core/assert.h"

#include "tgui/platform/application.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace tgui::platform
{
	application::application(const application::properties& props)
		: m_properties(props)
	{
		TGUI_ASSERT(!ms_instance, "app instance already exists.");
		ms_instance = this;
	}

	application::~application()
	{
		TGUI_ASSERT(ms_instance, "destroying app while no instance");
		ms_instance = nullptr;
	}

	void application::run()
	{
		init();

		while (!m_should_close)
		{
			update();

			//

			render();
		}

		shutdown();
	}

	void application::update()
	{
		m_window->process_events();

		on_update(m_time_step);
	}

	void application::render()
	{
		on_render(m_time_step);
	}

	tgui::platform::window* application::get_window() const
	{
		if (!m_window)
			return nullptr;

		return m_window.get();
	}

	void application::close()
	{
		m_should_close = true;
	}

	timestep application::get_timestep() const
	{
		return m_time_step;
	}

	timestep application::get_frametime() const
	{
		return m_frame_time;
	}

	float application::get_time() const
	{
		return (float)glfwGetTime();
	}

	void application::init()
	{
		m_window = std::make_unique<tgui::platform::window>(m_properties.window_properties);
		m_window->set_event_callback(make_delegate(&application::event_callback, this));

		subscribe(event_type::window_closed, tgui::make_event(&application::on_window_close, this));

		on_init();
	}

	void application::shutdown()
	{
		on_shutdown();
	}

	void application::event_callback(event& e)
	{
		dispatch(e);
	}

	void application::on_window_close(window_close_event& e)
	{
		m_should_close = true;
	}
}