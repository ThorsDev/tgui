#pragma once

#include <entt/entt.hpp>
#include "tgui/core/reference_object.h"
#include "tgui/ui/basic_components.h"

namespace tgui
{
	class view;

#define CREATE_FN(name) \
	friend class view; \
	name(view* _view, entt::entity handle) \
		: element(_view, handle) \
	{}\

	class element : public reference_object
	{
	public:
		element() = default;
		virtual ~element();

		void set_id(handle id);
		handle get_id() const;

		void set_name(const std::string& name);
		const std::string& get_name() const;

		void set_position(const glm::vec2& position);
		const glm::vec2& get_position() const;

		void set_size(const glm::vec2& size);
		const glm::vec2& get_size() const;

		void add_child(handle id);
		void remove_child(handle id);
		void set_parent(handle id);

		inline bool is_valid() const { return m_view != nullptr && m_handle != entt::null; }

	public:
		virtual void update() {};
		virtual void render() {};

	public:
		template<typename TComponent, typename... TArgs>
		TComponent& add_component(TArgs&&... args);

		template<typename TComponent>
		TComponent& get_component();

		template<typename TComponent>
		const TComponent& get_component() const;

		template<typename... TComponents>
		bool has_component();

		template<typename... TComponents>
		bool has_component() const;

	protected:
		friend class view;
		element(view* _view, entt::entity handle)
			: m_view(_view), m_handle(handle)
		{}

		void init();
		virtual void continue_init() {};

	protected:
		view* m_view = nullptr;
		entt::entity m_handle = entt::null;
	};
}