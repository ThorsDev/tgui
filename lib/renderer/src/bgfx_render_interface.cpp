#include "tgui/renderer/bgfx_render_interface.h"
#include "tgui/platform/application.h"
#include "tgui/math/math.h"

#include "tgui/renderer/bgfx_texture.h"
#include "tgui/renderer/bgfx_index_buffer.h"
#include "tgui/renderer/bgfx_vertex_buffer.h"

#include "embedded_shaders/quad_vs.bin.h"
#include "embedded_shaders/quad_fs.bin.h"
#include "embedded_shaders/quad_image_vs.bin.h"
#include "embedded_shaders/quad_image_fs.bin.h"
#include "embedded_shaders/fullscreen_vs.bin.h"
#include "embedded_shaders/fullscreen_fs.bin.h"

static const bgfx::EmbeddedShader s_embeddedQuadShaders[] =
{
	BGFX_EMBEDDED_SHADER(quad_vs),
	BGFX_EMBEDDED_SHADER(quad_fs),

	BGFX_EMBEDDED_SHADER_END()
};

static const bgfx::EmbeddedShader s_embeddedQuadImageShaders[] =
{
	BGFX_EMBEDDED_SHADER(quad_image_vs),
	BGFX_EMBEDDED_SHADER(quad_image_fs),

	BGFX_EMBEDDED_SHADER_END()
};

static const bgfx::EmbeddedShader s_embeddedFullscreenShaders[] =
{
	BGFX_EMBEDDED_SHADER(fullscreen_vs),
	BGFX_EMBEDDED_SHADER(fullscreen_fs),

	BGFX_EMBEDDED_SHADER_END()
};

namespace tgui::renderer
{
	static uint64_t PrimitiveTopologyToBGFX(primitive_topology::Enum prim)
	{
		switch (prim)
		{
			case primitive_topology::Triangles: return BGFX_STATE_NONE;
			case primitive_topology::TriangleStrip: return BGFX_STATE_PT_TRISTRIP;
			case primitive_topology::Lines: return BGFX_STATE_PT_LINES;
			case primitive_topology::LineStrip: return BGFX_STATE_PT_LINESTRIP;
			case primitive_topology::Points: return BGFX_STATE_PT_POINTS;
		}

		return BGFX_STATE_NONE;
	}

	static uint64_t WriteFlagsToBGFX(uint16_t flags)
	{
		uint64_t state = BGFX_STATE_NONE;
		if ((flags & write_flags::R) != 0)
			state |= BGFX_STATE_WRITE_R;
		if ((flags & write_flags::G) != 0)
			state |= BGFX_STATE_WRITE_G;
		if ((flags & write_flags::B) != 0)
			state |= BGFX_STATE_WRITE_B;
		if ((flags & write_flags::A) != 0)
			state |= BGFX_STATE_WRITE_A;
		if ((flags & write_flags::Z) != 0)
			state |= BGFX_STATE_WRITE_Z;

		return state;
	}

	bool bgfx_render_interface::on_init()
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

		m_quad_shader = new bgfx_embedded_shader(embedded_shader_properties{ "quad_vs", "quad_fs" }, s_embeddedQuadShaders);
		m_quad_shader->retain();

		m_quad_image_shader = new bgfx_embedded_shader(embedded_shader_properties{ "quad_image_vs", "quad_image_fs" }, s_embeddedQuadImageShaders);
		m_quad_image_shader->retain();

		m_fullscreen_shader = new bgfx_embedded_shader(embedded_shader_properties{ "fullscreen_vs", "fullscreen_fs" }, s_embeddedFullscreenShaders);
		m_fullscreen_shader->retain();

		glm::mat4 view(1.0f);
		glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(size.width), static_cast<float>(size.height), 0.0f, -10000.0f, 1000.0f);

		framebuffer_properties fbProps;
		fbProps.size = size;
		fbProps.attachments = {
			{ texture_format::RGBA, texture_usage::RenderTarget | texture_usage::MSAA_8X, nullptr },
		};

		m_framebuffer = new bgfx_frame_buffer(fbProps);
		m_framebuffer->retain();

		bgfx::resetView(0);
		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 1.0f, 0, 0x000000ff);
		bgfx::setViewTransform(0, glm::value_ptr(view), glm::value_ptr(proj));
		bgfx::setViewRect(0, 0, 0, size.width, size.height);

		bgfx::resetView(1);
		bgfx::setViewClear(1, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 1.0f, 0, 0x000000ff);
		bgfx::setViewRect(1, 0, 0, size.width, size.height);

		bgfx::resetView(2);
		bgfx::setViewClear(2, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 1.0f, 0, 0x000000ff);
		bgfx::setViewRect(2, 0, 0, size.width, size.height);

		m_quad_vertex_layout.begin()
			.add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();

		m_quad_image_vertex_layout.begin()
			.add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();

		m_quad_vertex_buffer_data.init(1024 * 4);

		m_quad_image_uniform = bgfx::createUniform("s_diffuseTexture", bgfx::UniformType::Sampler);

		texture_properties texProps;
		texProps.size = size;
		texProps.format = texture_format::RGBA;
		texProps.usage = texture_usage::RenderTarget | texture_usage::BlitDst;

		m_resolve_texture = new bgfx_texture(texProps);
		m_resolve_texture->retain();

		constexpr uint32_t quad_count = 5000;
		std::vector<uint32_t> indices(quad_count * 6);

		uint32_t offset = 0;
		for (int i = 0; i < quad_count * 6; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;
			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		m_quad_index_buffer = new bgfx_index_buffer(span::create(indices));
		m_quad_index_buffer->retain();

		{
			auto vtxBuf = new bgfx_dynamic_vertex_buffer(quad_count);
			vtxBuf->set_layout(m_quad_vertex_layout);

			m_quad_vertex_buffer = vtxBuf;
			m_quad_vertex_buffer->retain();
		}

		{
			std::vector<quad_image_vertex> vertices = {
				{ { -1.0f,  1.0f }, { 0.0f, 0.0f }, 0xffffffff },
				{ {  1.0f,  1.0f }, { 1.0f, 0.0f }, 0xffffffff },
				{ {  1.0f, -1.0f }, { 1.0f, 1.0f }, 0xffffffff },
				{ { -1.0f, -1.0f }, { 0.0f, 1.0f }, 0xffffffff }
			};

			auto vtxBuf = new bgfx_vertex_buffer(span::create(vertices));
			vtxBuf->set_layout(m_quad_image_vertex_layout);

			m_fullscreen_vertex_buffer = vtxBuf;
			m_fullscreen_vertex_buffer->retain();
		}

		return true;
	}

	void bgfx_render_interface::on_shutdown()
	{
		bgfx::shutdown();
	}

	void bgfx_render_interface::on_resize(const pixel_size& new_size)
	{
		glm::mat4 view(1.0f);
		glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(new_size.width), static_cast<float>(new_size.height), 0.0f, -1.0f, 1.0f);

		bgfx::reset(new_size.width, new_size.height, BGFX_RESET_NONE);
		bgfx::setViewTransform(0, glm::value_ptr(view), glm::value_ptr(proj));
		bgfx::setViewRect(0, 0, 0, new_size.width, new_size.height);

		bgfx::setViewClear(1, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 1.0f, 0, 0x000000ff);
		bgfx::setViewRect(1, 0, 0, new_size.width, new_size.height);

		bgfx::setViewClear(2, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 1.0f, 0, 0x000000ff);
		bgfx::setViewRect(2, 0, 0, new_size.width, new_size.height);
	}

	void bgfx_render_interface::on_frame()
	{
		bgfx::frame();
		m_pass_index = -1;
	}

	void bgfx_render_interface::draw(const draw_properties& properties)
	{
		if (!properties.vtx_buf)
			return;

		uint64_t state_flags = BGFX_STATE_NONE;

		if (properties.topology != primitive_topology::Triangles)
			state_flags |= PrimitiveTopologyToBGFX(properties.topology);
	
		state_flags |= WriteFlagsToBGFX(properties.write_flags);

		bgfx::setState(state_flags);

		if (properties.tex)
			bgfx::setTexture(0, m_quad_image_uniform, properties.tex->as<bgfx_texture>()->handle());

		if (properties.vertex_count > properties.start_vertex) {
			if (properties.vtx_buf->is_dynamic())
				bgfx::setVertexBuffer(0, properties.vtx_buf->as<bgfx_dynamic_vertex_buffer>()->handle(), properties.start_vertex, properties.vertex_count);
			else
				bgfx::setVertexBuffer(0, properties.vtx_buf->as<bgfx_vertex_buffer>()->handle(), properties.start_vertex, properties.vertex_count);
		}
		else {
			if (properties.vtx_buf->is_dynamic())
				bgfx::setVertexBuffer(0, properties.vtx_buf->as<bgfx_dynamic_vertex_buffer>()->handle());
			else
				bgfx::setVertexBuffer(0, properties.vtx_buf->as<bgfx_vertex_buffer>()->handle());
		}

		if (properties.index_count > properties.start_index)
			bgfx::setIndexBuffer(properties.idx_buf->as<bgfx_index_buffer>()->handle(), properties.start_index, properties.index_count);
		else
			bgfx::setIndexBuffer(properties.idx_buf->as<bgfx_index_buffer>()->handle());

		bgfx::submit(get_pass_index(), properties.program->as<bgfx_shader>()->handle());
	}

	void bgfx_render_interface::resolve(texture* target, framebuffer* source)
	{
		if (!m_resolve_texture->is_valid())
			return;

		if (!m_framebuffer->is_valid())
			return;

		ref<texture> srcTex = m_framebuffer->get_color_attachment(0);
		if (!srcTex || !srcTex->is_valid())
			return;

		bgfx::blit(get_pass_index(), m_resolve_texture->as<bgfx_texture>()->handle(), 0, 0, srcTex->as<bgfx_texture>()->handle());
	}

	uint16_t bgfx_render_interface::get_pass_index()
	{
		return m_pass_index;
	}

	void bgfx_render_interface::next_pass(framebuffer* fb)
	{
		m_pass_index++;

		if (fb)
			bgfx::setViewFrameBuffer(get_pass_index(), fb->as<bgfx_frame_buffer>()->handle());
		else
			bgfx::setViewFrameBuffer(get_pass_index(), BGFX_INVALID_HANDLE);

		bgfx::touch(m_pass_index);
	}
}