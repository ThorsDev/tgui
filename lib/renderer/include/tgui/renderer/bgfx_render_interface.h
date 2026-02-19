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

		virtual void draw(const draw_properties& properties) override;
		virtual void resolve(texture* target, framebuffer* source) override;
	
		virtual uint16_t get_pass_index() override;
		virtual void next_pass(framebuffer* fb = nullptr) override;

	protected:
		virtual bool on_init() override;
		virtual void on_shutdown() override;
		virtual void on_resize(const pixel_size& new_size) override;
		virtual void on_frame() override;

	private:
		bgfx::UniformHandle m_quad_image_uniform = BGFX_INVALID_HANDLE;

		bgfx::VertexLayout m_quad_vertex_layout;
		bgfx::VertexLayout m_quad_image_vertex_layout;

		int16_t m_pass_index = -1;
	};
}