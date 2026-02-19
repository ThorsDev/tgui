#include "tgui/renderer/bgfx_render_interface.h"
#include "tgui/platform/application.h"
#include "tgui/math/math.h"

#include "embedded_shaders/quad_vs.bin.h"
#include "embedded_shaders/quad_fs.bin.h"


static const bgfx::EmbeddedShader s_embeddedQuadShaders[] =
{
	BGFX_EMBEDDED_SHADER(quad_vs),
	BGFX_EMBEDDED_SHADER(quad_fs),

	BGFX_EMBEDDED_SHADER_END()
};

namespace tgui::renderer
{
	bool bgfx_render_interface::init()
	{
		tgui::platform::window* window = tgui::platform::application::get().get_window();
		if (!window)
			return false;

		auto const size = window->get_size();


		bgfx::PlatformData pd;
		memset(&pd, 0, sizeof(pd));
		pd.nwh = window->get_native_window();

		bgfx::Init init;
		init.type = bgfx::RendererType::Count; // auto-detect renderer
		init.platformData = pd;
		init.resolution.width = size.width;
		init.resolution.height = size.height;
		init.resolution.reset = BGFX_RESET_NONE;
		init.resolution.maxFrameLatency = 3;

		if (!bgfx::init(init))
			return false;

		m_quad_shader = tgui::ref<bgfx_embedded_shader>::create(embedded_shader_properties{ "quad_vs", "quad_fs" }, s_embeddedQuadShaders);

		glm::mat4 view(1.0f);
		glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(size.width), static_cast<float>(size.height), 0.0f, -10000.0f, 1000.0f);

		bgfx::resetView(0);
		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 1.0f, 0, 0x000000ff);
		bgfx::setViewTransform(0, glm::value_ptr(view), glm::value_ptr(proj));
		bgfx::setViewRect(0, 0, 0, size.width, size.height);

		m_quad_vertex_layout.begin()
			.add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();

		m_quad_vertex_buffer.init(1024 * 4);

		return true;
	}

	void bgfx_render_interface::shutdown()
	{
		m_quad_shader.reset();
		bgfx::shutdown();
	}

	void bgfx_render_interface::frame()
	{
		bgfx::touch(0);

		size_t quad_count = m_quad_vertex_buffer.size() / 4;
		size_t index_count = quad_count * 6;
		std::vector<uint16_t> indices(index_count);

		uint32_t offset = 0;
		for (uint32_t i = 0; i < index_count; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;
			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;
			offset += 4;
		}

		bgfx::TransientIndexBuffer tib;
		bgfx::TransientVertexBuffer tvb;

		bgfx::allocTransientIndexBuffer(&tib, quad_count * 6, false);
		bgfx::allocTransientVertexBuffer(&tvb, quad_count * 4, m_quad_vertex_layout);

		std::memcpy(tvb.data, m_quad_vertex_buffer.data(), sizeof(quad_vertex) * quad_count * 4);
		std::memcpy(tib.data, indices.data(), sizeof(uint16_t) * indices.size());

		bgfx::setVertexBuffer(0, &tvb);
		bgfx::setIndexBuffer(&tib);
		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
		bgfx::submit(0, m_quad_shader->handle());

		bgfx::frame();
		m_quad_vertex_buffer.advance();
	}

	void bgfx_render_interface::resize(const pixel_size& new_size)
	{
		glm::mat4 view(1.0f);
		glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(new_size.width), static_cast<float>(new_size.height), 0.0f, -1.0f, 1.0f);

		bgfx::reset(new_size.width, new_size.height, BGFX_RESET_NONE);
		bgfx::setViewTransform(0, glm::value_ptr(view), glm::value_ptr(proj));
		bgfx::setViewRect(0, 0, 0, new_size.width, new_size.height);
	}

	void bgfx_render_interface::draw_quad(const glm::vec2& pos, const glm::vec2& size, const color& color)
	{
		quad_vertex* vertices = m_quad_vertex_buffer.alloc(4);

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

	void bgfx_render_interface::draw_graph(const glm::vec2& pos, const glm::vec2& size, const std::array<glm::vec2, 4>& corners, const color& color)
	{
		quad_vertex* vertices = m_quad_vertex_buffer.alloc(4);

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
}