#pragma once

#include "tgui/core/base.h"
#include "tgui/event/event.h"

namespace tgui
{
	class window_close_event : public event
	{
	public:
		window_close_event() = default;

		static event_key get_static_type() { return event_type::window_closed; }
		virtual event_key get_event_type() const override { return get_static_type(); }
	};

	class window_minimize_event : public event
	{
	public:
		window_minimize_event(bool isMinimized)
			: m_minimized(isMinimized)
		{}

		static event_key get_static_type() { return event_type::window_minimized; }
		virtual event_key get_event_type() const override { return get_static_type(); }

		inline bool is_minimized() const { return m_minimized; }

	private:
		bool m_minimized = false;
	};

	class window_maximize_event : public event
	{
	public:
		window_maximize_event(bool isMaximized)
			: m_maximized(isMaximized)
		{}

		static event_key get_static_type() { return event_type::window_maximized; }
		virtual event_key get_event_type() const override { return get_static_type(); }

		inline bool is_maximized() const { return m_maximized; }

	private:
		bool m_maximized = false;
	};

	class window_resize_event : public event
	{
	public:
		window_resize_event(uint32_t width, uint32_t height)
			: m_width(width), m_height(height)
		{}

		static event_key get_static_type() { return event_type::window_resized; }
		virtual event_key get_event_type() const override { return get_static_type(); }

		inline uint32_t get_width() const { return m_width; }
		inline uint32_t get_height() const { return m_height; }

	private:
		uint32_t m_width, m_height;
	};

	class window_move_event : public event
	{
	public:
		window_move_event(uint32_t x, uint32_t y)
			: m_x(x), m_y(y)
		{}

		static event_key get_static_type() { return event_type::window_moved; }
		virtual event_key get_event_type() const override { return get_static_type(); }

		inline uint32_t get_x() const { return m_x; }
		inline uint32_t get_y() const { return m_y; }

	private:
		uint32_t m_x, m_y;
	};
}