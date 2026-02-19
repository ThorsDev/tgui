#pragma once

#include <tgui/core/base.h>
#include <tgui/core/handle.h>

#include <tgui/math/math.h>

namespace tgui
{
	struct id_component
	{
		handle id = 0;
	};

	struct tag_component 
	{
		std::string tag;

		tag_component() = default;
		tag_component(const tag_component& other) = default;
		tag_component(const std::string& tag)
			: tag(tag) {}

		operator std::string& () { return tag; }
		operator const std::string& () const { return tag; }
	};

	struct relationship_component
	{
		handle parent_id = 0;
		std::set<handle> children = {};

		relationship_component() = default;
		relationship_component(const relationship_component& other) = default;
		relationship_component(handle parent_id)
			: parent_id(parent_id) {}
	};

	struct transform_component
	{
		glm::vec2 position = { 0.0f, 0.0f };
		glm::vec2 size = { 0.0f, 0.0f };

		transform_component() = default;
		transform_component(const transform_component& other) = default;
		transform_component(const glm::vec2& position, const glm::vec2& size)
			: position(position), size(size) {}
	};

	// To identify elements that should be rendered
	struct render_component 
	{
		void* element;

		render_component(void* element)
			: element(element) {}
	};

	struct graph_component
	{
		std::array<glm::vec2, 4> corners;

		graph_component() = default;
		graph_component(const graph_component& other) = default;
		graph_component(const std::array<glm::vec2, 4>& corners)
			: corners(corners) {}
	};
}