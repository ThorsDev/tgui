#include "tgui/renderer/bgfx_embedded_shader.h"

namespace tgui::renderer
{
	bgfx_embedded_shader::bgfx_embedded_shader(const embedded_shader_properties& props, const bgfx::EmbeddedShader* emb_shader)
	{
		bgfx::RendererType::Enum type = bgfx::getRendererType();

		bgfx::ShaderHandle vsh = bgfx::createEmbeddedShader(emb_shader, type, props.vs_name.c_str());
		bgfx::ShaderHandle fsh = bgfx::createEmbeddedShader(emb_shader, type, props.fs_name.c_str());

		m_program_handle = bgfx::createProgram(vsh, fsh, true);
	}

	bgfx_embedded_shader::~bgfx_embedded_shader()
	{
	}

}