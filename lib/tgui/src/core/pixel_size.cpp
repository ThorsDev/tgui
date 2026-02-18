#include "tgui/core/pixel_size.h"

namespace tgui
{
	pixel_size::pixel_size()
		: width(0), height(0)
	{}

	pixel_size::pixel_size(int32_t scalar)
		: width(scalar), height(scalar)
	{}

	pixel_size::pixel_size(int32_t w, int32_t h)
		: width(w), height(h)
	{}
}