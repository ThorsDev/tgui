#pragma once

#include "tgui/core/base.h"
#include <eventpp/eventdispatcher.h>

namespace tgui
{
	using event_key = uint16_t;

	struct event_type
	{
		enum Enum : uint16_t
		{
			noop = 0,
			key_pressed, key_released, key_typed,
			mouse_pressed, mouse_released, mouse_moved, mouse_scrolled,
			window_closed, window_minimized, window_maximized, window_resized, 
			window_focused, window_lost_focus, window_moved,
		};
	};

	class event
	{
	public:
		bool prevented = false;

		virtual ~event() {}
		virtual event_key get_event_type() const = 0;

		inline void prevent_default() { prevented = true; }
	};

	using event_dispatcher = eventpp::EventDispatcher<event_key, void(event*), eventpp::MultipleThreading>;

	template <typename C, typename EventT>
	auto make_event(void (C::*method)(EventT&), C* obj)
	{
		static_assert(std::is_base_of<event, EventT>::value, "EventT must derive from event");

		return [obj, method](event* e) -> void 
		{
			if (e->prevented)
				return;

			(obj->*method)(static_cast<EventT&>(*e));
		};
	}

	class event_listener
	{
	public:
		event_listener() = default;
		~event_listener() = default;

	public:
		inline void dispatch(event& e)
		{
			m_dispatcher.dispatch(e.get_event_type(), &e);
		}

		inline void subscribe(event_key key, std::function<void(event*)> callback)
		{
			m_dispatcher.appendListener(key, callback);
		}

	private:
		event_dispatcher m_dispatcher;
	};
}