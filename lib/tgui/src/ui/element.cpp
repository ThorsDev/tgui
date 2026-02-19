#include "tgui/ui/element.h"
#include "tgui/ui/view.h"
#include <format>

namespace tgui
{
	void element::init()
	{
		add_component<id_component>(handle());
		add_component<tag_component>(std::format("unknown_element_{}", m_view->m_element_counter++));
		add_component<relationship_component>();
		add_component<transform_component>();

		continue_init();
	}

	element::~element()
	{
	}

	void element::set_id(handle id)
	{
		get_component<id_component>().id = id;
	}

	handle element::get_id() const
	{
		return get_component<id_component>().id;
	}

	void element::set_name(const std::string& name)
	{
		m_view->rename_element(get_name(), name);
		get_component<tag_component>().tag = name;
	}

	const std::string& element::get_name() const
	{
		return get_component<tag_component>().tag;
	}

	void element::set_position(const glm::vec2& position)
	{
		get_component<transform_component>().position = position;
	}

	const glm::vec2& element::get_position() const
	{
		return get_component<transform_component>().position;
	}

	void element::set_size(const glm::vec2& size)
	{
		get_component<transform_component>().size = size;
	}

	const glm::vec2& element::get_size() const
	{
		return get_component<transform_component>().size;
	}

	void element::add_child(handle id)
	{
		handle my_id = get_id();

		element* child = m_view->find_element_by_id(id);
		if (!child || !child->is_valid())
			return;

		relationship_component& relationship = get_component<relationship_component>();
		relationship.children.insert(id);

		relationship_component& child_relationship = child->get_component<relationship_component>();
		child_relationship.parent_id = my_id;
	}

	void element::remove_child(handle id)
	{
		handle my_id = get_id();
		element* child = m_view->find_element_by_id(id);

		if (!child || !child->is_valid())
			return;

		relationship_component& child_relationship = child->get_component<relationship_component>();
		if (child_relationship.parent_id != my_id)
			return;

		child_relationship.parent_id = 0;

		relationship_component& relationship = get_component<relationship_component>();
		relationship.children.erase(id);
	}

	void element::set_parent(handle id)
	{
		relationship_component& relationship = get_component<relationship_component>();
		if (relationship.parent_id != 0)
		{
			element* parent = m_view->find_element_by_id(relationship.parent_id);
			if (parent && parent->is_valid())
				parent->remove_child(get_id());
		}

		element* new_parent = m_view->find_element_by_id(id);
		if (!new_parent || !new_parent->is_valid())
			return;

		new_parent->add_child(get_id());
	}
}