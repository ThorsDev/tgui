#pragma once

#include "tgui/core/base.h"

namespace tgui
{
	class handle
	{
	public:
		handle();
		handle(uint64_t hndl);
		handle(const handle& other);

		operator uint64_t() { return m_handle; }
		operator const uint64_t() const { return m_handle; }

	private:
		uint64_t m_handle;
	};
}

namespace std {

	template <>
	struct hash<tgui::handle>
	{
		size_t operator()(const tgui::handle& handle) const
		{
			return handle;
		}
	};
}