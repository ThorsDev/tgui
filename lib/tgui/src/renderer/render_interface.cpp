#include "tgui/renderer/render_interface.h"

namespace tgui
{
	void render_interface::resize(const pixel_size & new_size)
	{
		m_framebuffer->resize(new_size);
		m_resolve_texture->resize(new_size);

		on_resize(new_size);
	}

	void render_interface::frame()
	{
		next_pass(m_framebuffer);

		// quads
		{
			size_t quad_count = m_quad_vertex_buffer_data.size() / 4;
			size_t index_count = quad_count * 6;

			m_quad_vertex_buffer->set_data(span::create(m_quad_vertex_buffer_data.data(), sizeof(quad_vertex) * quad_count * 4), 0, false);

			draw_properties drawProps;
			drawProps.vtx_buf = m_quad_vertex_buffer;
			drawProps.idx_buf = m_quad_index_buffer;
			drawProps.program = m_quad_shader;
			drawProps.topology = primitive_topology::Triangles;

			draw(drawProps);
		}

		next_pass();

		// resolve
		{
			resolve(m_resolve_texture, m_framebuffer);
		}

		next_pass();

		// present
		{
			draw_fullscreen_quad(m_resolve_texture);
		}

		on_frame();

		m_quad_vertex_buffer_data.advance();
		m_quad_image_vertex_buffer_data.advance();
	}

	void render_interface::draw_quad(const glm::vec2& pos, const glm::vec2& size, const color& color)
	{
		quad_vertex* vertices = m_quad_vertex_buffer_data.alloc(4);

		vertices->position = pos;
		vertices->color = color.bgra();
		vertices++;

		vertices->position = pos + glm::vec2(size.x, 0.0f);
		vertices->color = color.bgra();
		vertices++;

		vertices->position = pos + size;
		vertices->color = color.bgra();
		vertices++;

		vertices->position = pos + glm::vec2(0.0f, size.y);
		vertices->color = color.bgra();
	}

	void render_interface::draw_graph(const glm::vec2& pos, const glm::vec2& size, const std::array<glm::vec2, 4>& corners, const color& color)
	{
		quad_vertex* vertices = m_quad_vertex_buffer_data.alloc(4);

		vertices->position = pos + corners[0];
		vertices->color = color.bgra();
		vertices++;

		vertices->position = pos + glm::vec2(size.x, 0.0f) + corners[1];
		vertices->color = color.bgra();
		vertices++;

		vertices->position = pos + size + corners[2];
		vertices->color = color.bgra();
		vertices++;

		vertices->position = pos + glm::vec2(0.0f, size.y) + corners[3];
		vertices->color = color.bgra();
	}

	void render_interface::draw_fullscreen_quad(texture* tex)
	{
		draw_properties drawProps;
		drawProps.vtx_buf = m_fullscreen_vertex_buffer;
		drawProps.idx_buf = m_quad_index_buffer;
		drawProps.tex = tex;
		drawProps.program = m_fullscreen_shader;
		drawProps.topology = primitive_topology::Triangles;
		drawProps.start_index = 0;
		drawProps.index_count = 6;
		
		draw(drawProps);
	}

	bool render_interface::init()
	{
		if (!on_init())
			return false;

		return true;
	}

	void render_interface::shutdown()
	{
		if (m_quad_shader) {
			m_quad_shader->release();
			m_quad_shader = nullptr;
		}

		if (m_quad_image_shader) {
			m_quad_image_shader->release();
			m_quad_image_shader = nullptr;
		}

		if (m_fullscreen_shader) {
			m_fullscreen_shader->release();
			m_fullscreen_shader = nullptr;
		}

		if (m_framebuffer) {
			m_framebuffer->release();
			m_framebuffer = nullptr;
		}

		if (m_resolve_texture) {
			m_resolve_texture->release();
			m_resolve_texture = nullptr;
		}

		if (m_quad_index_buffer) {
			m_quad_index_buffer->release();
			m_quad_index_buffer = nullptr;
		}

		if (m_quad_vertex_buffer) {
			m_quad_vertex_buffer->release();
			m_quad_vertex_buffer = nullptr;
		}

		if (m_quad_image_vertex_buffer) {
			m_quad_image_vertex_buffer->release();
			m_quad_image_vertex_buffer = nullptr;
		}

		if (m_fullscreen_vertex_buffer) {
			m_fullscreen_vertex_buffer->release();
			m_fullscreen_vertex_buffer = nullptr;
		}

		on_shutdown();
	}
}