#pragma once

#include "tgui/renderer/vertex_buffer.h"
#include <bgfx/bgfx.h>

namespace tgui::renderer
{
	class bgfx_vertex_buffer_base : public vertex_buffer
	{
	public:
		bgfx_vertex_buffer_base() = default;
		bgfx_vertex_buffer_base(span data) : vertex_buffer(data) {}
		virtual ~bgfx_vertex_buffer_base() = default;

		virtual bool is_dynamic() const { return false; }

		void set_layout(const bgfx::VertexLayout layout) 
		{ 
			m_bgfx_layout = layout; 
			on_set_layout();
		}

	private:
		virtual void on_set_layout() = 0;

	protected:
		bgfx::VertexLayout m_bgfx_layout = {};
	};

	class bgfx_vertex_buffer : public bgfx_vertex_buffer_base
	{
	public:
		bgfx_vertex_buffer(span data);
		virtual ~bgfx_vertex_buffer();

		virtual bool is_dynamic() const override { return false; }
		virtual bool is_valid() const override;
		virtual void set_data(span data, uint32_t, bool) override;

		inline bgfx::VertexBufferHandle handle() const { return m_handle; }

	private:
		virtual void on_set_layout() override;

	protected:
		virtual void create() override;
		virtual void destroy() override;

	private:
		bgfx::VertexBufferHandle m_handle = BGFX_INVALID_HANDLE;
	};

	class bgfx_dynamic_vertex_buffer : public bgfx_vertex_buffer_base
	{
	public:
		bgfx_dynamic_vertex_buffer(uint32_t num);
		bgfx_dynamic_vertex_buffer(span data);
		virtual ~bgfx_dynamic_vertex_buffer();

		virtual bool is_dynamic() const override { return true; }
		virtual bool is_valid() const override;
		virtual void set_data(span data, uint32_t startVertex, bool copy) override;

		inline bgfx::DynamicVertexBufferHandle handle() const { return m_handle; }

	private:
		virtual void on_set_layout() override;

	protected:
		virtual void create() override;
		virtual void destroy() override;

	private:
		size_t m_num = 0;
		bgfx::DynamicVertexBufferHandle m_handle = BGFX_INVALID_HANDLE;
	};
}