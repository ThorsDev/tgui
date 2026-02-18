#pragma once

#include "tgui/core/base.h"
#include "tgui/platform/window_adapter.h"

#include "tgui/core/pixel_position.h"
#include "tgui/core/pixel_size.h"

#include "tgui/core/reference_object.h"

namespace tgui
{
	class render_interface : public reference_object
	{
	public:
		~render_interface() = default;

		virtual bool init() = 0;
		virtual void shutdown() = 0;
		virtual void frame() = 0;
		virtual void resize(const pixel_size& new_size) = 0;

		virtual void draw_quad(const pixel_position& pos, const pixel_size& size) = 0;
	};
}