#include "tgui/core/assert.h"
#include "tgui/renderer/bgfx_vertex_buffer.h"

namespace tgui::renderer
{
	bgfx_vertex_buffer::bgfx_vertex_buffer(span data)
		: bgfx_vertex_buffer_base(data)
	{}

	bgfx_vertex_buffer::~bgfx_vertex_buffer()
	{
		destroy();
	}

	bool bgfx_vertex_buffer::is_valid() const
	{
		return bgfx::isValid(m_handle);
	}

	void bgfx_vertex_buffer::set_data(span data, uint32_t, bool)
	{
		// here we can just assume if the layout's stride is equals to the spans stride the vertex layout matches with the data.
		if (!m_data.empty())
			TGUI_ASSERT(m_bgfx_layout.m_stride == data.stride(), "Vertex buffer layout stride does not match the data stride!");

		m_data = data;

		destroy();
		create();
	}

	void bgfx_vertex_buffer::on_set_layout()
	{
		// we only create the vertex buffer when the layout is set.
		create();
	}

	void bgfx_vertex_buffer::create()
	{
		if (m_data.empty())
			return;

		m_handle = bgfx::createVertexBuffer(bgfx::copy(m_data.data(), m_data.size()), m_bgfx_layout);
		m_data = {};
	}

	void bgfx_vertex_buffer::destroy()
	{
		if (is_valid())
			bgfx::destroy(m_handle);
	}

	bgfx_dynamic_vertex_buffer::bgfx_dynamic_vertex_buffer(uint32_t num)
		: m_num(num)
	{
	}

	bgfx_dynamic_vertex_buffer::bgfx_dynamic_vertex_buffer(span data)
		: bgfx_vertex_buffer_base(data), m_num(data.size())
	{
	}
	
	bgfx_dynamic_vertex_buffer::~bgfx_dynamic_vertex_buffer()
	{
		destroy();
	}

	bool bgfx_dynamic_vertex_buffer::is_valid() const
	{
		return bgfx::isValid(m_handle);
	}

	void bgfx_dynamic_vertex_buffer::set_data(span data, uint32_t startVertex, bool copy)
	{
		TGUI_ASSERT(m_bgfx_layout.m_stride != 0, "Vertex buffer layout must be set before setting data!");
		if (!data.empty())
			TGUI_ASSERT(m_bgfx_layout.m_stride == data.stride(), "Vertex buffer layout stride does not match the data stride!");

		if (data.size() > m_num)
		{
			TGUI_ASSERT(false, "Data size exceeds the size of the dynamic vertex buffer!");
			return;
		}

		const bgfx::Memory* mem = nullptr;
		if (copy)
			mem = bgfx::copy(data.data(), data.size());
		else
			mem = bgfx::makeRef(data.data(), data.size());

		bgfx::update(m_handle, startVertex, mem);
	}

	void bgfx_dynamic_vertex_buffer::on_set_layout()
	{
		create();
	}

	void bgfx_dynamic_vertex_buffer::create()
	{
		TGUI_ASSERT(m_num > 0, "Dynamic vertex buffer must have a positive number of vertices!");

		m_handle = bgfx::createDynamicVertexBuffer(m_num, m_bgfx_layout);
		if (!m_data.empty())
			bgfx::update(m_handle, 0, bgfx::copy(m_data.data(), m_data.size()));
	}

	void bgfx_dynamic_vertex_buffer::destroy()
	{
		if (is_valid())
			bgfx::destroy(m_handle);
	}
}
