#pragma once

#include "tgui/core/base.h"
#include "tgui/event/event.h"
#include "tgui/platform/input_codes.h"

namespace tgui
{
	class mouse_event : public event
	{
	public:
		inline mouse_button get_mouse_button() const { return m_mouse_button; }

	protected:
		mouse_event(mouse_button mouse)
			: m_mouse_button(mouse)
		{ }

		mouse_button m_mouse_button;
	};

	class mouse_pressed_event : public mouse_event
	{
	public:
		mouse_pressed_event(mouse_button mouse)
			: mouse_event(mouse)
		{}

		static event_key get_static_type() { return event_type::mouse_pressed; }
		virtual event_key get_event_type() const override { return get_static_type(); }
	};

	class mouse_released_event : public mouse_event
	{
	public:
		mouse_released_event(mouse_button mouse)
			: mouse_event(mouse)
		{}

		static event_key get_static_type() { return event_type::mouse_released; }
		virtual event_key get_event_type() const override { return get_static_type(); }
	};

	class mouse_moved_event : public event
	{
	public:
		mouse_moved_event(int x, int y)
			: m_mouse_x(x), m_mouse_y(y)
		{}

		inline int get_mouse_x() const { return m_mouse_x; }
		inline int get_mouse_y() const { return m_mouse_y; }

		static event_key get_static_type() { return event_type::mouse_moved; }
		virtual event_key get_event_type() const override { return get_static_type(); }

	private:
		int m_mouse_x, m_mouse_y;
	};

	class mouse_scrolled_event : public event
	{
	public:
		mouse_scrolled_event(float x, float y)
			: m_offset_x(x), m_offset_y(y)
		{}

		inline float get_x_offset() const { return m_offset_x; }
		inline float get_y_offset() const { return m_offset_y; }

		static event_key get_static_type() { return event_type::mouse_scrolled; }
		virtual event_key get_event_type() const override { return get_static_type(); }

	private:
		float m_offset_x, m_offset_y;
	};
}