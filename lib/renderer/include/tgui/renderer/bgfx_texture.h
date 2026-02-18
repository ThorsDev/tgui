#pragma once

#include "tgui/renderer/texture.h"
#include <bgfx/bgfx.h>

namespace tgui::renderer
{
	class bgfx_texture : public texture
	{
	public:
		bgfx_texture(const texture_properties& props);
		bgfx_texture(const void* data, size_t size);
		virtual ~bgfx_texture();

	public:
		virtual bool is_valid() const override;
		virtual void resize(const pixel_size& new_size) override;
		virtual void set_data(const void* data, size_t size) override;

		inline bgfx::TextureHandle handle() const { return m_texture_handle; }

	protected:
		virtual void create_from_memory(const void* data, size_t size) override;
		virtual void create() override;
		virtual void destroy() override;

	private:
		bgfx::TextureHandle m_texture_handle = BGFX_INVALID_HANDLE;
	};
}