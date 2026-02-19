#pragma once

#include <array>
#include <vector>

namespace tgui
{
	template<typename T, int MAX_BUFFER = 3>
	class rotating_buffer
	{
	public:
		rotating_buffer() = default;
		~rotating_buffer() = default;

		void init(size_t buffer_size)
		{
			m_buffers.fill(std::vector<T>(buffer_size));
			m_write_pos = 0;
		}

		T* alloc(size_t count)
		{
			T* write_point = get_current_buffer().data() + m_write_pos;
			m_write_pos += count;

			return write_point;
		}

		T* data()
		{
			return get_current_buffer().data();
		}

		size_t size() const
		{
			return m_write_pos;
		}

		size_t max_size() const
		{
			return get_current_buffer().size();
		}

		void advance()
		{
			m_current_buffer = (m_current_buffer + 1) % MAX_BUFFER;

			auto current_buffer = get_current_buffer();
			m_write_pos = 0;
		}

	protected:
		std::vector<T>& get_current_buffer() { return m_buffers.at(m_current_buffer); }
		const std::vector<T>& get_current_buffer() const { return m_buffers.at(m_current_buffer); }

	private:
		int m_current_buffer = 0;
		std::array<std::vector<T>, MAX_BUFFER> m_buffers;

		size_t m_write_pos = 0;
	};
}