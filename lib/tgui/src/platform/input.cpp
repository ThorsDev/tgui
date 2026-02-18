#include "tgui/platform/input.h"
#include "tgui/platform/input_codes.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace tgui
{
	input::context* input::ms_context = nullptr;
	
	void input::init(tgui::window_adapter* wnd)
	{
		ms_context = new input::context();
		ms_context->window = wnd;
	}

	void input::shutdown()
	{
		delete ms_context;
		ms_context = nullptr;
	}

	bool input::is_key_state(key_code key, key_state state)
	{
		return ms_context->key_states[key].state == state;
	}

	bool input::is_mouse_state(mouse_button mouse, key_state state)
	{
		return ms_context->mouse_states[mouse].state == state;
	}

	void input::update_key_state(key_code key, key_state state)
	{
		auto& rState = ms_context->key_states[key];
		rState.old_state = rState.state;
		rState.state = state;
	}

	void input::update_mouse_state(mouse_button mouse, key_state state)
	{
		auto& rState = ms_context->mouse_states[mouse];
		rState.old_state = rState.state;
		rState.state = state;
	}

	pixel_position input::get_mouse_position()
	{
		return ms_context->window->get_mouse_pos();
	}
}