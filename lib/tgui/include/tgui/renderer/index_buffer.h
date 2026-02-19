#pragma once

#include <tgui/core/reference_object.h>
#include <tgui/core/span.h>

namespace tgui
{
	class index_buffer : public reference_object
	{
	public:
		index_buffer() = default;
		index_buffer(span data) : m_data(data) {}
		virtual ~index_buffer() = default;

		virtual void set_data(span data) = 0;
		virtual bool is_valid() const = 0;

	protected:
		virtual void create() = 0;
		virtual void destroy() = 0;

	protected:
		span m_data;
	};
}