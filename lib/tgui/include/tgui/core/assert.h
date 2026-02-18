#pragma once

#include "tgui/core/base.h"
#include "tgui/log/log.h"

#if TGUI_PLATFORM_WINDOWS
	#define TGUI_DBG_BREAK __debugbreak()
#elif defined(__clang__) || defined(__GNUC__)
	#define TGUI_BREAK __builtin_trap()
#else
	#include <signal.h>
	#define TGUI_DBG_BREAK raise(SIGTRAP)
#endif

#if defined(_DEBUG)
	#define TGUI_ENABLE_ASSERTS
#endif

#define TGUI_ENABLE_VERIFY

#if defined(TGUI_ENABLE_ASSERTS)
	#define TGUI_ASSERT_MESSAGE_INTERNAL(...) tgui::log::print_assert_message("Assertion Failed", ##__VA_ARGS__)
	#define TGUI_ASSERT(condition, ...) { if(!(condition)) { TGUI_ASSERT_MESSAGE_INTERNAL(__VA_ARGS__); TGUI_DBG_BREAK; } }
#else
	#define TGUI_ASSERT(condition, ...)
#endif

#if defined(TGUI_ENABLE_VERIFY)
	#define TGUI_VERIFY_MESSAGE_INTERNAL(...)  tgui::log::print_assert_message("Verify Failed", ##__VA_ARGS__)
	#define TGUI_VERIFY(condition, ...) { if(!(condition)) { TGUI_VERIFY_MESSAGE_INTERNAL(__VA_ARGS__); TGUI_DBG_BREAK; } }
#else
	#define TGUI_VERIFY(condition, ...)
#endif