#pragma once

#include "tgui/core/base.h"
#include "tgui/renderer/shader.h"

#include <bgfx/bgfx.h>
#include <bgfx/embedded_shader.h>

namespace tgui
{
	struct embedded_shader_properties
	{
		std::string vs_name;
		std::string fs_name;
	};

	class bgfx_embedded_shader : public shader
	{
	public:
		bgfx_embedded_shader(const embedded_shader_properties& props, const bgfx::EmbeddedShader* emb_shader);
		virtual ~bgfx_embedded_shader();

	public:
		virtual bool is_valid() const override;

		inline bgfx::ProgramHandle handle() const { return m_program_handle; }

	protected:
		virtual void destroy() override;

	private:
		bgfx::ProgramHandle m_program_handle = BGFX_INVALID_HANDLE;
	};
}