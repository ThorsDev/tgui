#pragma once

#include "tgui/core/base.h"
#include "tgui/core/timestep.h"
#include "tgui/core/reference_object.h"

#include <entt/entt.hpp>
#include "element.h"
#include "graph.h"

namespace tgui
{
	class view : public reference_object
	{
	public:
		view();
		virtual ~view();

		template<typename TElem>
		TElem* create_element()
		{
			static_assert(std::is_base_of_v<element, TElem>, "TElem must be derived from element.");

			TElem* elem = new TElem(this, m_registry.create());
			elem->init();
			elem->retain();

			m_element_map[elem->get_id()] = elem;
			m_name_map[elem->get_name()] = elem;
			return elem;
		}

		element* find_element_by_id(handle id);
		element* find_element_by_name(const std::string& name);

		void remove_element(handle id);

		void update(timestep ts);
		void render(timestep ts);

	private:
		void rename_element(const std::string& old_name, const std::string& new_name);

	private:
		friend class element;
		entt::registry m_registry;

		uint64_t m_element_counter = 0;

		std::unordered_map<handle, element*> m_element_map;
		std::unordered_map<std::string, element*> m_name_map;
	};
}

#include "element_impl.h"