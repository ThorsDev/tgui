#pragma once

#include <tgui/core/base.h>
#include <tgui/event/event.h>

#include <tgui/core/pixel_size.h>
#include <tgui/core/pixel_position.h>

namespace tgui
{
	class window_adapter
	{
	public:
		struct properties
		{
			std::string title = "TGUI Window";
			pixel_size size = { 640, 480 };
			bool decorated = true;
			bool fullscreen = false;
			bool vsync = true;
		};

	public:
		window_adapter(const properties& props)
			: m_props(props)
		{}

		~window_adapter() = default;

		virtual void on_event(event& ev) = 0;

		virtual void set_mouse_pos(const pixel_position& pos) = 0;
		virtual pixel_position get_mouse_pos() const = 0;
		
		virtual void set_size(const pixel_size& size) = 0;
		virtual pixel_size get_size() const = 0;

		virtual void set_position(const pixel_position& pos) = 0;
		virtual pixel_position get_position() const = 0;

		virtual void set_title(const std::string& title) = 0;
		virtual const std::string& get_title() const = 0;

		virtual bool is_minimized() const = 0;
		virtual bool is_maximized() const = 0;

		virtual void show() = 0;
		virtual void hide() = 0;

	protected:
		window_adapter::properties m_props = {};
	};
}