#pragma once

#include <atomic>
#include <cstddef>
#include <type_traits>

namespace tgui
{
	class reference_object
	{
	public:
		inline size_t get_reference_count() const noexcept { return m_ref_count.load(); }

		inline void retain() noexcept
		{
			inc_ref_count();
		}

		inline void release() noexcept
		{
			dec_ref_count();

			if (0 == get_reference_count())
				destruct();
		}

		template<typename TOther>
		TOther* as() noexcept
		{
			return static_cast<TOther*>(this);
		}

	protected:
		inline void inc_ref_count() noexcept { ++m_ref_count; }
		inline void dec_ref_count() noexcept { --m_ref_count; }

		virtual void destruct() { delete this; }

	private:
		mutable std::atomic<uint32_t> m_ref_count = 0;
	};
}