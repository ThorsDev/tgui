#pragma once

#include "tgui/core/base.h"
#include "tgui/event/event.h"
#include "tgui/platform/input_codes.h"

namespace tgui
{
	class key_event : public event
	{
	public:
		inline key_code get_key_code() const { return m_key_code; }

	protected:
		key_event(key_code keycode)
			: m_key_code(keycode)
		{}

		key_code m_key_code;
	};

	class key_pressed_event : public key_event
	{
	public:
		key_pressed_event(key_code keycode, int repeatCount)
			: key_event(keycode), m_repeat_count(repeatCount)
		{}

		static event_key get_static_type() { return event_type::key_pressed; }
		virtual event_key get_event_type() const override { return get_static_type(); }

		inline int get_repeat_count() const { return m_repeat_count; }

	private:
		int m_repeat_count;
	};

	class key_released_event : public key_event
	{
	public:
		key_released_event(key_code keycode)
			: key_event(keycode)
		{}

		static event_key get_static_type() { return event_type::key_released; }
		virtual event_key get_event_type() const override { return get_static_type(); }
	};

	class key_typed_event : public key_event
	{
	public:
		key_typed_event(key_code keycode)
			: key_event(keycode)
		{}

		static event_key get_static_type() { return event_type::key_typed; }
		virtual event_key get_event_type() const override { return get_static_type(); }
	};
}