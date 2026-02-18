#include "tgui/core/pixel_position.h"

namespace tgui
{
	pixel_position::pixel_position()
		: x(0), y(0)
	{}

	pixel_position::pixel_position(int32_t scalar)
		: x(scalar), y(scalar)
	{}

	pixel_position::pixel_position(int32_t x_, int32_t y_)
		: x(x_), y(y_)
	{}
}