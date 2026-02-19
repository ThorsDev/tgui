#pragma once

#include "tgui/core/base.h"
#include "tgui/platform/window_adapter.h"

#include "tgui/core/pixel_position.h"
#include "tgui/core/pixel_size.h"

#include "tgui/core/reference_object.h"

#include "tgui/renderer/draw_commands.h"

#include "tgui/math/math.h"
#include "tgui/math/color.h"

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

		virtual void draw_quad(const glm::vec2& pos, const glm::vec2& size, const color& color) = 0;
		virtual void draw_graph(const glm::vec2& pos, const glm::vec2& size, const std::array<glm::vec2, 4>& corners, const color& color) = 0;
	};
}