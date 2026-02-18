#include <filesystem>

#define SPDLOG_USE_STD_FORMAT
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "tgui/log/log.h"

namespace tgui
{
	void log::init(const log::init_info& init)
	{
		std::filesystem::path p(init.file_path);
		std::string fallback_file_name = "log.log";

		bool is_file = false;
		if (p.has_extension()) {
			is_file = true;
			fallback_file_name = p.filename().string();

			p = p.parent_path();
		}

		if (std::filesystem::is_directory(p) && !std::filesystem::exists(p))
			std::filesystem::create_directories(p);

		std::vector<spdlog::sink_ptr> sinks;
		sinks.push_back(
			std::make_shared<spdlog::sinks::basic_file_sink_mt>(std::format("{0}/{1}", p.string(), fallback_file_name).data(), true)
		);

		sinks[0]->set_pattern("[%T] [%l] %n: %v");

		if (init.has_console) {
			sinks.push_back(
				std::make_shared<spdlog::sinks::stdout_color_sink_mt>()
			);

			sinks[1]->set_pattern("%^[%T] %n: %v%$");
		}

		ms_internal_logger = std::make_shared<spdlog::logger>(init.name, sinks.begin(), sinks.end());
		ms_internal_logger->set_level(spdlog::level::trace);
	}

	void log::shutdown()
	{ 
		ms_internal_logger.reset();
		spdlog::drop_all();
	}

	void log::register_tag(const std::string& tag_name, log::level level)
	{
		auto& detail = ms_tag_details[tag_name];
		detail.level_filter = level;
	}

	void log::enable_tag(const std::string& tag_name)
	{
		auto it = ms_tag_details.find(tag_name);
		if (it != ms_tag_details.end())
			it->second.enabled = true;
	}

	void log::disable_tag(const std::string& tag_name)
	{
		auto it = ms_tag_details.find(tag_name);
		if (it != ms_tag_details.end())
			it->second.enabled = false;
	}
}