#include "tgui/renderer/bgfx_texture.h"

namespace tgui::renderer
{
	static bgfx::TextureFormat::Enum convert_texture_format(texture_format::Enum format)
	{
		switch (format)
		{
		case texture_format::RGB:
			return bgfx::TextureFormat::RGB8;
		case texture_format::RGBA:
			return bgfx::TextureFormat::RGBA8;
		case texture_format::RGBA16F:
			return bgfx::TextureFormat::RGBA16F;
		case texture_format::RGBA32F:
			return bgfx::TextureFormat::RGBA32F;
		case texture_format::D16:
			return bgfx::TextureFormat::D16;
		case texture_format::D24:
			return bgfx::TextureFormat::D24;
		case texture_format::D24S8:
			return bgfx::TextureFormat::D24S8;
		case texture_format::D32:
			return bgfx::TextureFormat::D32;
		case texture_format::D16F:
			return bgfx::TextureFormat::D16F;
		case texture_format::D32F:
			return bgfx::TextureFormat::D32F;
		case texture_format::D0S8:
			return bgfx::TextureFormat::D0S8;
		default:
			return bgfx::TextureFormat::Unknown;
		}
	}

	static void parse_usage_flags(uint64_t usage, uint64_t& out_flags)
	{
		if (usage & texture_usage::RenderTarget)
			out_flags |= BGFX_TEXTURE_RT;

		if (usage & texture_usage::SRGB)
			out_flags |= BGFX_TEXTURE_SRGB;
	}

	bgfx_texture::bgfx_texture(const texture_properties& props)
		: texture(props)
	{}

	bgfx_texture::bgfx_texture(const void* data, size_t size)
		: texture(data, size)
	{}

	bgfx_texture::~bgfx_texture()
	{}

	bool bgfx_texture::is_valid() const
	{
		return bgfx::isValid(m_texture_handle);
	}

	void bgfx_texture::resize(const pixel_size & new_size)
	{
		if (new_size == m_properties.size)
			return;

		m_properties.size = new_size;
		
		destroy();
		create();
	}

	void bgfx_texture::set_data(const void* data, size_t size)
	{
		if (!is_valid())
			return;

		bgfx::updateTexture2D(m_texture_handle, 0, 0, 0, 0, m_properties.size.width, m_properties.size.height, bgfx::copy(data, size));
	}

	void bgfx_texture::create_from_memory(const void* data, size_t size)
	{
	}

	void bgfx_texture::create()
	{
		uint64_t flags = 0;
		parse_usage_flags(m_properties.usage, flags);

		m_texture_handle = bgfx::createTexture2D(
			m_properties.size.width, 
			m_properties.size.height, 
			m_properties.mip_count > 1, 
			m_properties.layer_count, 
			convert_texture_format(m_properties.format),
			flags
		);
	}

	void bgfx_texture::destroy()
	{
		if (bgfx::isValid(m_texture_handle))
			bgfx::destroy(m_texture_handle);
	}
}