#pragma once

#include <cstdint>
#include <memory>
#include <functional>
#include <random>
#include <mutex>

#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>

namespace tgui
{
	template <class TClass, class... TArgs>
	auto make_delegate(void (TClass::*method)(TArgs...), TClass* obj)
		-> std::function<void(TArgs...)>
	{
		return [obj, method](TArgs... args)
		{
			(obj->*method)(std::forward<TArgs>(args)...);
		};
	}
}