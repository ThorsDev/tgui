#include "tgui/core/assert.h"
#include "tgui/renderer/renderer.h"

namespace tgui
{
	void static_renderer::init(render_interface* render_interface)
	{
		ms_render_interface = render_interface;
		if (ms_render_interface)
			ms_render_interface->retain();
	}

	void static_renderer::shutdown()
	{
		if (ms_render_interface)
		{
			ms_render_interface->release();
			ms_render_interface = nullptr;
		}
	}

	void static_renderer::draw_quad(const glm::vec2& pos, const glm::vec2& size)
	{
		TGUI_ASSERT(ms_render_interface, "render interface is not defined, maybe init is not called.");
		ms_render_interface->draw_quad(pos, size);
	}

	void static_renderer::draw_graph(const glm::vec2& pos, const glm::vec2& size, const std::array<glm::vec2, 4>& corners)
	{
		TGUI_ASSERT(ms_render_interface, "render interface is not defined, maybe init is not called.");
		ms_render_interface->draw_graph(pos, size, corners);
	}
}
