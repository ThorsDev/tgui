#pragma once

#include <tgui/renderer/framebuffer.h>
#include "bgfx/bgfx.h"

namespace tgui::renderer
{
	class bgfx_frame_buffer : public framebuffer
	{
	public:
		bgfx_frame_buffer(const framebuffer_properties& props);
		virtual ~bgfx_frame_buffer();

		virtual bool is_valid() const override;
		virtual void resize(const pixel_size& new_size) override;

		inline bgfx::FrameBufferHandle handle() const { return m_handle; }

	protected:
		virtual void create() override;
		virtual void destroy() override;

	private:
		bgfx::FrameBufferHandle m_handle = BGFX_INVALID_HANDLE;
	};
}