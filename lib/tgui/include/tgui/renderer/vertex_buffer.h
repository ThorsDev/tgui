#pragma once

#include <tgui/core/reference_object.h>
#include <tgui/core/span.h>

namespace tgui
{
	class vertex_buffer : public reference_object
	{
	public:
		vertex_buffer() = default;
		vertex_buffer(span data) : m_data(data) {}
		virtual ~vertex_buffer() = default;

		virtual bool is_dynamic() const = 0;
		virtual void set_data(span data, uint32_t startVertex, bool copy) = 0;
		virtual bool is_valid() const = 0;

	protected:
		virtual void create() = 0;
		virtual void destroy() = 0;

	protected:
		span m_data = {};
	};
}