#pragma once

#include "tgui/renderer/render_interface.h"

#include "tgui/core/reference.h"
#include "tgui/core/rotating_buffer.h"

#include "tgui/renderer/bgfx_embedded_shader.h"
#include "tgui/renderer/bgfx_framebuffer.h"

#include "tgui/renderer/index_buffer.h"
#include "tgui/renderer/vertex_buffer.h"

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

		virtual void draw_quad(const glm::vec2& pos, const glm::vec2& size, const color& color) override;
		virtual void draw_graph(const glm::vec2& pos, const glm::vec2& size, const std::array<glm::vec2, 4>& corners, const color& color) override;
	
	protected:
		void render_fullscreen_quad(bgfx::ViewId id, texture* tex);

	private:
		tgui::ref<bgfx_embedded_shader> m_quad_shader = nullptr;
		tgui::ref<bgfx_embedded_shader> m_quad_image_shader = nullptr;
		tgui::ref<bgfx_embedded_shader> m_fullscreen_shader = nullptr;

		index_buffer* m_quad_index_buffer = nullptr;
		vertex_buffer* m_quad_vertex_buffer = nullptr;

		bgfx::UniformHandle m_quad_image_uniform = BGFX_INVALID_HANDLE;

		tgui::ref<bgfx_frame_buffer> m_frame_buffer = nullptr;
		tgui::ref<texture> m_resolve_texture = nullptr;

		bgfx::VertexLayout m_quad_vertex_layout;
		bgfx::VertexLayout m_quad_image_vertex_layout;

		rotating_buffer<quad_vertex> m_quad_vertex_buffer_data;
	};
}