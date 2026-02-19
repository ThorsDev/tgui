#pragma once

#include "tgui/core/pixel_size.h"
#include "tgui/core/reference_object.h"

namespace tgui
{
	struct texture_usage
	{
		enum Enum : uint64_t
		{
			Default = (1 << 0),
			RenderTarget = (1 << 1),
			SRGB = (1 << 2),
			MSAA_2X = (1 << 3),
			MSAA_4X = (1 << 4),
			MSAA_8X = (1 << 5),
			BlitDst = (1 << 6),
		};
	};

	struct texture_format
	{
		enum Enum
		{
			Unknown,

			// Color Formats
			RGB,
			RGBA,
			RGBA16F,
			RGBA32F,

			// Depth Formats
			D16,
			D24,
			D24S8,
			D32,
			D16F,
			D24F,
			D32F,
			D0S8
		};
	};

	struct texture_properties
	{
		std::string name = "";
		pixel_size size = {};
		uint32_t mip_count = 1;
		uint32_t layer_count = 1;

		// texture_format::...
		texture_format::Enum format = texture_format::Unknown;

		// texture_usage::...
		uint32_t usage = texture_usage::Default;
	};

	class texture : public reference_object
	{
	public:
		texture() = default;
		texture(const texture_properties& props) : m_properties(props) {}
		virtual ~texture() = default;

	public:
		virtual bool is_valid() const = 0;
		virtual void resize(const pixel_size& new_size) = 0;
		virtual void set_data(const void* data, size_t size) = 0;

	protected:
		virtual void create_from_memory(const void* data, size_t size) = 0;
		virtual void create() = 0;
		virtual void destroy() = 0;

	protected:
		texture_properties m_properties = {};
	};
}