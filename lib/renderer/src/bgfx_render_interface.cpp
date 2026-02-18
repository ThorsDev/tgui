#include "tgui/renderer/bgfx_render_interface.h"
#include "tgui/platform/application.h"

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

		bgfx::resetView(0);
		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 1.0f, 0, 0x000000ff);
		bgfx::setViewRect(0, 0, 0, size.width, size.height);
		bgfx::setViewScissor(0, 0, 0, size.width, size.height);

		m_quad_vertex_layout.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();

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

		struct Vertex
		{
			float x, y, z;
			uint32_t color;
		};

		static std::vector<uint16_t> indices = { 0, 1, 2, 2, 3, 0 };
		static std::vector<Vertex> vertices = {
			{ -0.5f, -0.5f, 0.0f, 0xffff0000 },
			{  0.5f, -0.5f, 0.0f, 0xff00ff00 },
			{  0.5f,  0.5f, 0.0f, 0xff0000ff },
			{ -0.5f,  0.5f, 0.0f, 0xffffff00 }
		};

		bgfx::TransientIndexBuffer tib;
		bgfx::TransientVertexBuffer tvb;

		bgfx::allocTransientIndexBuffer(&tib, 6, false);
		bgfx::allocTransientVertexBuffer(&tvb, 4, m_quad_vertex_layout);

		memcpy(tib.data, indices.data(), 6 * sizeof(uint16_t));
		memcpy(tvb.data, vertices.data(), 4 * sizeof(Vertex));

		bgfx::setVertexBuffer(0, &tvb);
		bgfx::setIndexBuffer(&tib);
		bgfx::setState(BGFX_STATE_DEFAULT);
		bgfx::submit(0, m_quad_shader->handle());

		bgfx::frame();
	}

	void bgfx_render_interface::resize(const pixel_size& new_size)
	{
		bgfx::reset(new_size.width, new_size.height, BGFX_RESET_NONE);
		bgfx::setViewRect(0, 0, 0, new_size.width, new_size.height);
		bgfx::setViewScissor(0, 0, 0, new_size.width, new_size.height);
	}

	void bgfx_render_interface::draw_quad(const pixel_position& pos, const pixel_size& size)
	{
		m_quads.push_back(std::make_pair(pos, size));
	}
}