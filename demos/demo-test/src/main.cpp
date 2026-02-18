#include <tgui/platform/application.h>

int main() 
{
	tgui::log::init_info log_props;
	log_props.name = "TGUI";
	log_props.file_path = "tgui.log";
	log_props.has_console = true;

	tgui::log::init(log_props);

	tgui::platform::application::properties app_props;
	std::unique_ptr<tgui::platform::application> app = std::make_unique<tgui::platform::application>(app_props);
	app->run();

	tgui::log::shutdown();

	return EXIT_SUCCESS;
}