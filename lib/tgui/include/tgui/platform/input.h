#pragma once

#include "tgui/core/base.h"
#include "tgui/platform/input_codes.h"

#include "tgui/platform/window_adapter.h"

namespace tgui
{
	struct input_state
	{
		key_state state = key_state::noop;
		key_state old_state = key_state::noop;
	};

	class input
	{
	public:
		struct context
		{
			window_adapter*							window;
			std::map<key_code, input_state>			key_states; 
			std::map<mouse_button, input_state>		mouse_states; 
		};

	public:
		static void init(tgui::window_adapter* wnd);
		static void shutdown();

		static bool is_key_state(key_code key, key_state state);
		static bool is_mouse_state(mouse_button mouse, key_state state);

		static void update_key_state(key_code key, key_state state);
		static void update_mouse_state(mouse_button mouse, key_state state);

		static pixel_position get_mouse_position();

	private:
		static context* ms_context;
	};
}