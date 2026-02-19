#pragma once

#include <tgui/math/math.h>

namespace tgui
{
	struct quad_vertex
	{
		glm::vec2 position;
		uint32_t color;
	};

	struct quad_image_vertex
	{
		glm::vec2 position;
		glm::vec2 uv;
		uint32_t color;
	};
}