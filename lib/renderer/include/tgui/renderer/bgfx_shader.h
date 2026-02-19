#pragma once

#include "tgui/core/base.h"
#include "tgui/renderer/shader.h"

#include <bgfx/bgfx.h>

namespace tgui::renderer
{
	class bgfx_shader : public shader
	{
	public:
		bgfx_shader() = default;
		virtual ~bgfx_shader() = default;

	public:
		virtual bool is_valid() const override;
		inline bgfx::ProgramHandle handle() const { return m_program_handle; }

	protected:
		virtual void destroy() override;

	protected:
		bgfx::ProgramHandle m_program_handle = BGFX_INVALID_HANDLE;
	};
}