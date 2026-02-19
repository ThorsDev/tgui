#pragma once

#include "tgui/renderer/render_interface.h"

namespace tgui
{
	class static_renderer
	{
	public:
		static void init(render_interface* render_interface);
		static void shutdown();

		static void draw_quad(const glm::vec2& pos, const glm::vec2& size, const color& color);
		static void draw_graph(const glm::vec2& pos, const glm::vec2& size, const std::array<glm::vec2, 4>& corners, const color& color);

	private:
		inline static render_interface* ms_render_interface = nullptr;
	};
}