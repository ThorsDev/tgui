#pragma once

#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <format>
#include <utility>

#define SPDLOG_USE_STD_FORMAT
#include <spdlog/spdlog.h>

namespace tgui
{
	class log
	{
	public:
		enum class level
		{
			trace = 0, info, warn, error, fatal
		};

		struct tag_details
		{
			bool enabled = true;
			log::level level_filter = level::trace;
		};

		struct init_info
		{
			std::string name;
			std::string file_path;

			bool has_console;
		};

	public:
		static void init(const log::init_info& init);
		static void shutdown();

		static void register_tag(const std::string& tag_name, log::level level);
		static void enable_tag(const std::string& tag_name);
		static void disable_tag(const std::string& tag_name);

	public:
		template<typename... TArgs>
		static void print_message(log::level lvl, std::format_string<TArgs...> format, TArgs&&... args);

		static void print_message(log::level lvl, std::string_view msg);

		template<typename... TArgs>
		static void print_message_with_tag(log::level lvl, std::string_view tag, std::format_string<TArgs...> format, TArgs&&... args);

		static void print_message_with_tag(log::level lvl, std::string_view tag, std::string_view msg);

		template<typename... TArgs>
		static void print_assert_message(std::string_view prefix, std::format_string<TArgs...> message, TArgs&&... args);

		static void print_assert_message(std::string_view prefix);

	private:
		inline static std::shared_ptr<spdlog::logger>& get_internal_logger() { return ms_internal_logger; }

	private:
		inline static std::shared_ptr<spdlog::logger> ms_internal_logger = nullptr;
		inline static std::map<std::string, log::tag_details> ms_tag_details = {};
		
	};

	template<typename... TArgs>
	inline void log::print_message(log::level lvl, std::format_string<TArgs...> format, TArgs&&... args)
	{
		auto logger = get_internal_logger();
		if (!logger)
			return;

		switch (lvl)
		{
		case log::level::trace:
			logger->warn(format, std::forward<TArgs>(args)...);
			break;
		case log::level::info:
			logger->info(format, std::forward<TArgs>(args)...);
			break;
		case log::level::warn:
			logger->warn(format, std::forward<TArgs>(args)...);
			break;
		case log::level::error:
			logger->error(format, std::forward<TArgs>(args)...);
			break;
		case log::level::fatal:
			logger->critical(format, std::forward<TArgs>(args)...);
			break;
		}
	}

	inline void log::print_message(log::level lvl, std::string_view msg)
	{
		auto logger = get_internal_logger();
		if (!logger)
			return;

		switch (lvl)
		{
		case log::level::trace:
			logger->warn(msg);
			break;
		case log::level::info:
			logger->info(msg);
			break;
		case log::level::warn:
			logger->warn(msg);
			break;
		case log::level::error:
			logger->error(msg);
			break;
		case log::level::fatal:
			logger->critical(msg);
			break;
		}
	}

	template<typename... TArgs>
	inline void log::print_message_with_tag(log::level lvl, std::string_view tag, std::format_string<TArgs...> format, TArgs&&... args)
	{
		//auto it = ms_tag_details.find(tag.data());
		//if (it == ms_tag_details.end())
		//	return;

		//if (!it->second.enabled)
		//	return;

		//if (it->second.level_filter <= lvl)
		//	return;

		auto logger = get_internal_logger();
		if (!logger)
			return;

		print_message(lvl, "[{0}] {1}", tag, std::format(format, std::forward<TArgs>(args)...));
	}

	inline void log::print_message_with_tag(log::level lvl, std::string_view tag, std::string_view msg)
	{
		//auto it = ms_tag_details.find(tag.data());
		//if (it == ms_tag_details.end())
		//	return;

		//if (!it->second.enabled)
		//	return;

		//if (it->second.level_filter <= lvl)
		//	return;

		auto logger = get_internal_logger();
		if (!logger)
			return;

		print_message(lvl, "[{0}] {1}", tag, msg);
	}

	template<typename... TArgs>
	inline void log::print_assert_message(std::string_view prefix, std::format_string<TArgs...> message, TArgs&&... args)
	{
		auto logger = get_internal_logger();
		if (!logger)
			return;

		auto formatted = std::format(message, std::forward<TArgs>(args)...);
		logger->error("{0}: {1}", prefix, formatted);

		// TODO: message boxes
#if HAS_ASSERT_MESSAGE_BOX
		MessageBoxA(nullptr, formatted.data(), "Noir Assert", MB_OK | MB_ICONERROR);
#endif
	}

	inline void log::print_assert_message(std::string_view prefix)
	{
		auto logger = get_internal_logger();
		logger->error("{0}", prefix);
	#if HAS_ASSERT_MESSAGE_BOX
		MessageBoxA(nullptr, "No message :(", "Noir Assert", MB_OK | MB_ICONERROR);
	#endif
	}
}

#define TGUI_TRACE_TAG(tag, ...) tgui::log::print_message_with_tag(tgui::log::level::trace, tag, __VA_ARGS__)
#define TGUI_INFO_TAG(tag, ...) tgui::log::print_message_with_tag(tgui::log::level::info, tag, __VA_ARGS__)
#define TGUI_WARN_TAG(tag, ...) tgui::log::print_message_with_tag(tgui::log::level::warn, tag, __VA_ARGS__)
#define TGUI_ERROR_TAG(tag, ...) tgui::log::print_message_with_tag(tgui::log::level::error, tag, __VA_ARGS__)
#define TGUI_FATAL_TAG(tag, ...) tgui::log::print_message_with_tag(tgui::log::level::fatal, tag, __VA_ARGS__)

#define TGUI_TRACE(...) tgui::log::print_message(tgui::log::level::trace, __VA_ARGS__)
#define TGUI_INFO(...) tgui::log::print_message(tgui::log::level::info, __VA_ARGS__)
#define TGUI_WARN(...) tgui::log::print_message(tgui::log::level::warn, __VA_ARGS__)
#define TGUI_ERROR(...) tgui::log::print_message(tgui::log::level::error, __VA_ARGS__)
#define TGUI_FATAL(...) tgui::log::print_message(tgui::log::level::fatal, __VA_ARGS__)