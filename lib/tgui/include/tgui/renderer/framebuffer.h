#pragma once

#include "tgui/core/assert.h"
#include "tgui/core/reference.h"
#include "tgui/renderer/texture.h"

namespace tgui
{
	struct attachment_properties
	{
		texture_format::Enum format = texture_format::Unknown;
		uint64_t usage = texture_usage::RenderTarget;

		texture* existing = nullptr;
	};

	struct framebuffer_properties
	{
		pixel_size size;
		std::vector<attachment_properties> attachments;
	};

	class framebuffer : public reference_object
	{
	public:
		framebuffer(const framebuffer_properties& props) : m_properties(props) {}
		virtual ~framebuffer() = default;

		ref<texture> get_color_attachment(size_t index)
		{
			TGUI_ASSERT(index < m_color_attachments.size(), "color attachment index out of range");
			return m_color_attachments[index];
		}

		ref<texture> get_depth_attachment()
		{
			return m_depth_attachment;
		}

	public:
		virtual bool is_valid() const = 0;
		virtual void resize(const pixel_size& new_size) = 0;

	protected:
		virtual void create() = 0;
		virtual void destroy() = 0;

	protected:
		framebuffer_properties m_properties;

		std::vector<texture*> m_color_attachments;
		texture* m_depth_attachment = nullptr;
	};
}