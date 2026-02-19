#pragma once

#include <vector>

namespace tgui
{
	class span
	{
	public:
		span() = default;
		span(const void* data, size_t size, size_t stride)
			: m_data(data), m_size(size), m_stride(stride)
		{}

		size_t size() const { return m_size; }
		size_t size_bytes() const { return m_size / m_stride; }
		size_t stride() const { return m_stride; }

		bool empty() const { return m_size == 0; }
		const void* data() const { return m_data; }

		template<typename T>
		inline static span create(const std::vector<T>& data)
		{
			return span(data.data(), data.size() * sizeof(T), sizeof(T));
		}

		template<typename T>
		inline static span create(const T* data, size_t count)
		{
			return span(data, count * sizeof(T), sizeof(T));
		}

	private:
		const void* m_data = nullptr;
		size_t m_size = 0;
		size_t m_stride = 0;
	};
}