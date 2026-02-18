#pragma once

#include "tgui/renderer/render_interface.h"

#include "tgui/core/reference.h"
#include "tgui/renderer/bgfx_embedded_shader.h"

namespace tgui::renderer
{
	class bgfx_render_interface : public render_interface
	{
	public:
		bgfx_render_interface() = default;
		virtual ~bgfx_render_interface() = default;

		virtual bool init() override;
		virtual void shutdown() override;
		virtual void frame() override;
		virtual void resize(const pixel_size& new_size) override;

		virtual void draw_quad(const pixel_position& pos, const pixel_size& size) override;
	
	private:
		tgui::ref<bgfx_embedded_shader> m_quad_shader = nullptr;

		bgfx::VertexLayout m_quad_vertex_layout;
		std::vector<std::pair<pixel_position, pixel_size>> m_quads;
	};
}