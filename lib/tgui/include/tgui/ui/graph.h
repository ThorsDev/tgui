#pragma once

#include "tgui/ui/element.h"

namespace tgui
{
	class graph : public element
	{
	public:
		graph() = default;
		virtual ~graph();

		void set_color(const color& color) { m_color = color; }
		const color& get_color() const { return m_color;}

		glm::vec2& corner(int index);
		const std::array<glm::vec2, 4>& get_corners() const;

	public:
		virtual void update() override;
		virtual void render() override;

	protected:
		CREATE_FN(graph);

		virtual void continue_init() override;

	private:
		color m_color;
	};
}