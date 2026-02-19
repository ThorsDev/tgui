#pragma once

namespace tgui
{
	template<typename TComponent, typename... TArgs>
	TComponent& element::add_component(TArgs&&... args)
	{
		return m_view->m_registry.emplace<TComponent>(m_handle, std::forward<TArgs>(args)...);
	}

	template<typename TComponent>
	TComponent& element::get_component()
	{
		return m_view->m_registry.get<TComponent>(m_handle);
	}

	template<typename TComponent>
	const TComponent& element::get_component() const
	{
		return m_view->m_registry.get<TComponent>(m_handle);
	}

	template<typename... TComponents>
	bool element::has_component()
	{
		return m_view->m_registry.all_of<TComponents...>(m_handle);
	}

	template<typename... TComponents>
	bool element::has_component() const
	{
		return m_view->m_registry.all_of<TComponents...>(m_handle);
	}
}