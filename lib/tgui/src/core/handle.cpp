#include "tgui/core/handle.h"

namespace tgui
{
	static std::random_device s_random_device;
	static std::mt19937_64 s_eng(s_random_device());
	static std::uniform_int_distribution<uint64_t> s_uniform_distribution;

	handle::handle()
		: m_handle(s_uniform_distribution(s_eng))
	{
	}

	handle::handle(uint64_t hndl)
		: m_handle(hndl)
	{
	}

	handle::handle(const handle& other)
		: m_handle(other.m_handle)
	{
	}
}
