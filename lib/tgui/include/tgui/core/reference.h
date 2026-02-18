#pragma once

#include <cassert>
#include "reference_object.h"

namespace tgui
{
	template<typename T>
	class ref
	{
	public:
		ref() : m_ptr(nullptr) {}
		ref(std::nullptr_t) : m_ptr(nullptr) {};

		ref(T* ptr) : m_ptr(ptr)
		{
			static_assert(std::is_base_of<reference_object, T>::value, "T must derive from reference_object");
			
			if (m_ptr)
				m_ptr->retain();
		}

		ref(const ref& other) : m_ptr(other.m_ptr)
		{
			if (m_ptr)
				m_ptr->retain();
		}

		ref(ref&& other) : m_ptr(other.m_ptr)
		{
			other.m_ptr = nullptr;
		}

		~ref()
		{
			if (m_ptr)
				m_ptr->release();
		}

		ref& operator=(const ref& other)
		{
			reset(other.m_ptr);
			return *this;
		}

		void reset(T* ptr = nullptr)
		{
			if (ptr == m_ptr)
				return;

			if (ptr)
				ptr->retain();

			if (m_ptr)
				m_ptr->release();

			m_ptr = ptr;
		}

		T* get() const 
		{ 
			return m_ptr; 
		}

		T& operator*() const
		{
			assert(m_ptr != nullptr && "can't dereference a null pointer");
			return *m_ptr;
		}

		T* operator->() const
		{
			assert(m_ptr != nullptr && "ref is a nullptr");
			return m_ptr;
		}

		bool operator==(const ref<T>& other) const
		{
			return m_ptr == other.m_ptr;
		}

		bool operator!=(const ref<T>& other) const
		{
			return !(*this == other);
		}

		operator bool() { return m_ptr != nullptr; }
		operator bool() const { return m_ptr != nullptr; }

		template<typename... TArgs>
		static ref<T> create(TArgs&&... args)
		{
			return ref<T>(new T(std::forward<TArgs>(args)...));
		}

	private:
		T* m_ptr = nullptr;
	};
}