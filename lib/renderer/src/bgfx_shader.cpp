#include "tgui/renderer/bgfx_shader.h"

namespace tgui::renderer
{
	bool bgfx_shader::is_valid() const
	{
		return bgfx::isValid(m_program_handle);
	}

	void bgfx_shader::destroy()
	{
		if (is_valid())
			bgfx::destroy(m_program_handle);
	}
}