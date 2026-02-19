#include "tgui/renderer/bgfx_framebuffer.h"
#include "tgui/renderer/bgfx_texture.h"

namespace tgui::renderer
{
	static bool is_depth_format(texture_format::Enum format)
	{
		switch (format)
		{
			case texture_format::Enum::D16:
			case texture_format::Enum::D24:
			case texture_format::Enum::D24S8:
			case texture_format::Enum::D32:
			case texture_format::Enum::D16F:
			case texture_format::Enum::D24F:
			case texture_format::Enum::D32F:
			case texture_format::Enum::D0S8:
				return true;
		}

		return false;
	}

	bgfx_frame_buffer::bgfx_frame_buffer(const framebuffer_properties& props)
		: framebuffer(props)
	{
		create();
	}

	bgfx_frame_buffer::~bgfx_frame_buffer()
	{
		destroy();
	}

	bool bgfx_frame_buffer::is_valid() const
	{
		return bgfx::isValid(m_handle);
	}

	void bgfx_frame_buffer::resize(const pixel_size& new_size)
	{
		if (new_size == m_properties.size)
			return;

		m_properties.size = new_size;

		destroy();
		create();
	}

	void bgfx_frame_buffer::create()
	{
		std::vector<bgfx::Attachment> attachments;

		for (auto& attachment : m_properties.attachments)
		{
			bgfx::Attachment bgfxAttachment;
			texture* tex = nullptr;

			if (attachment.existing)
			{
				tex = attachment.existing;
				bgfxAttachment.init(tex->as<bgfx_texture>()->handle(), bgfx::Access::Write, 0);
			}
			else
			{
				TGUI_ASSERT(attachment.usage & texture_usage::RenderTarget, "attachment must have RenderTarget usage flag");

				texture_properties texProps;
				texProps.size = m_properties.size;
				texProps.format = attachment.format;
				texProps.usage = attachment.usage;

				tex = new bgfx_texture(texProps);
				bgfxAttachment.init(tex->as<bgfx_texture>()->handle(), bgfx::Access::Write, 0);
			}

			tex->retain();

			if (is_depth_format(attachment.format))
			{
				TGUI_ASSERT(m_depth_attachment == nullptr, "multiple depth attachments are not supported");
				m_depth_attachment = tex;
			}
			else
			{
				m_color_attachments.push_back(tex);
			}

			attachments.push_back(bgfxAttachment);
		}

		m_handle = bgfx::createFrameBuffer((uint8_t)attachments.size(), attachments.data(), false);
	}

	void bgfx_frame_buffer::destroy()
	{
		if (is_valid())
			bgfx::destroy(m_handle);

		for (auto& attachment : m_color_attachments)
			if (attachment)
				attachment->release();

		m_color_attachments.clear();

		if (m_depth_attachment)
		{
			m_depth_attachment->release();
			m_depth_attachment = nullptr;
		}
	}
}