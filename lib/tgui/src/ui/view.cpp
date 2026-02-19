#include "tgui/ui/view.h"

namespace tgui
{
	view::view()
	{
	}

	view::~view()
	{
	}

	element* view::find_element_by_id(handle id)
	{
		auto f = m_element_map.find(id);
		if (f != m_element_map.end())
			return f->second;

		return nullptr;
	}

	element* view::find_element_by_name(const std::string& name)
	{
		auto f = m_name_map.find(name);
		if (f != m_name_map.end())
			return f->second;

		return nullptr;
	}

	void view::remove_element(handle id)
	{
		auto f = m_element_map.find(id);
		if (f != m_element_map.end())
		{
			element* elem = f->second;
			m_name_map.erase(elem->get_name());
			m_element_map.erase(f);

			elem->release();
		}
	}

	void view::update(timestep ts)
	{
	}

	void view::render(timestep ts)
	{
		auto elements = m_registry.view<id_component, render_component>();
		elements.each([this](auto entity, const id_component& id, const render_component& render)
		{
			element* elem = (element*)render.element;
			if (elem)
				elem->render();
		});
	}

	void view::rename_element(const std::string& old_name, const std::string& new_name)
	{
		auto f = m_name_map.find(old_name);
		if (f != m_name_map.end())
		{
			element* elem = f->second;
			m_name_map.erase(f);
			m_name_map[new_name] = elem;
		}
	}
}