#pragma once

#include "tgui/renderer/index_buffer.h"
#include <bgfx/bgfx.h>

namespace tgui::renderer
{
	class bgfx_index_buffer : public index_buffer
	{
	public:
		bgfx_index_buffer() = default;
		bgfx_index_buffer(span data);
		virtual ~bgfx_index_buffer();

		virtual void set_data(span data) override;
		virtual bool is_valid() const override;

		inline bgfx::IndexBufferHandle handle() const { return m_handle; }

	protected:
		virtual void create() override;
		virtual void destroy() override;

	private:
		bgfx::IndexBufferHandle m_handle = BGFX_INVALID_HANDLE;
	};
}