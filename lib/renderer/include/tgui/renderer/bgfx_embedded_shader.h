#pragma once

#include "bgfx_shader.h"

#include <bgfx/bgfx.h>
#include <bgfx/embedded_shader.h>

namespace tgui::renderer
{
	struct embedded_shader_properties
	{
		std::string vs_name;
		std::string fs_name;
	};

	class bgfx_embedded_shader : public bgfx_shader
	{
	public:
		bgfx_embedded_shader(const embedded_shader_properties& props, const bgfx::EmbeddedShader* emb_shader);
		virtual ~bgfx_embedded_shader();
	};
}