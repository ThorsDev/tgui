#pragma once

#include "tgui/core/reference_object.h"

namespace tgui
{
	class shader : public reference_object
	{
	public:
		~shader()
		{
			destroy();
		}

	public:
		virtual bool is_valid() const = 0;

	protected:
		virtual void destroy() {};
	};
}