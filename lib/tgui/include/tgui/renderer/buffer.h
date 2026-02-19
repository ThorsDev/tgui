#pragma once

#include <cstdint>

namespace tgui
{
	struct buffer_usage
	{
		enum Enum : uint16_t
		{
			UsageImmutable,
			UsageDynamic
		};
	};

	struct buffer_properties
	{
		buffer_usage::Enum usage;
		size_t stride;
	};
}