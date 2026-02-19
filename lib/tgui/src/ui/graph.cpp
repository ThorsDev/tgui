#include "tgui/ui/view.h"
#include "tgui/ui/graph.h"

#include "tgui/renderer/renderer.h"

namespace tgui
{
	graph::~graph()
	{
	}

	glm::vec2& graph::corner(int index)
	{
		if (index < 0 || index >= 4)
			throw std::out_of_range("corner index must be between 0 and 3");

		return get_component<graph_component>().corners.at(index);
	}

	const std::array<glm::vec2, 4>& graph::get_corners() const
	{
		return get_component<graph_component>().corners;
	}

	void graph::update()
	{
	}

	void graph::render()
	{
		const auto& transform = get_component<transform_component>();
		const auto& graph_data = get_component<graph_component>();

		static_renderer::draw_graph(transform.position, transform.size, get_corners(), m_color);
	}

	void graph::continue_init()
	{
		add_component<render_component>(this);
		add_component<graph_component>();

		m_color = color(0xffffffff);
	}
}