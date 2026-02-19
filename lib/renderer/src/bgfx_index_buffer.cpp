#include "tgui/renderer/bgfx_index_buffer.h"

namespace tgui::renderer
{
	bgfx_index_buffer::bgfx_index_buffer(span data)
		: index_buffer(data)
	{
		create();
	}

	bgfx_index_buffer::~bgfx_index_buffer()
	{
		destroy();
	}

	void bgfx_index_buffer::set_data(span data)
	{
		m_data = data;

		destroy();
		create();
	}

	bool bgfx_index_buffer::is_valid() const
	{
		return bgfx::isValid(m_handle);
	}

	void bgfx_index_buffer::create()
	{
		if (m_data.empty())
			return;

		uint32_t flags = 0;
		if (m_data.stride() == sizeof(uint32_t))
			flags |= BGFX_BUFFER_INDEX32;

		m_handle = bgfx::createIndexBuffer(bgfx::copy(m_data.data(), m_data.size()), flags);
		m_data = {};
	}

	void bgfx_index_buffer::destroy()
	{
		if (is_valid())
			bgfx::destroy(m_handle);
	}
}