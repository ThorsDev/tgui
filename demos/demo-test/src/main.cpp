#include <tgui/platform/application.h>

#include <tgui/renderer/bgfx_render_interface.h>
#include <tgui/event/window_events.h>

class application : public tgui::platform::application
{
public:
	application(const tgui::platform::application::properties& props)
		: tgui::platform::application(props)
	{}

	virtual void on_update(tgui::timestep ts) override
	{
	}

	virtual void on_render(tgui::timestep ts) override
	{
		m_render_interface->frame();
	}

protected:
	virtual void on_init() override
	{
		subscribe(tgui::event_type::window_resized, tgui::make_event(&application::on_resize, this));

		m_render_interface = tgui::ref<tgui::renderer::bgfx_render_interface>::create();
		if (!m_render_interface->init())
		{
			TGUI_ERROR("Failed to initialize render interface.");
			close();
		}
	}

	virtual void on_shutdown() override
	{
		if (m_render_interface)
			m_render_interface->shutdown();
	}

	void on_resize(tgui::window_resize_event& event)
	{
		if (m_render_interface)
			m_render_interface->resize({(int32_t)event.get_width(), (int32_t)event.get_height()});
	}

private:
	tgui::ref<tgui::renderer::bgfx_render_interface> m_render_interface = nullptr;
};

int main() 
{
	tgui::log::init_info log_props;
	log_props.name = "TGUI";
	log_props.file_path = "tgui.log";
	log_props.has_console = true;

	tgui::log::init(log_props);

	tgui::platform::application::properties app_props;
	std::unique_ptr<application> app = std::make_unique<application>(app_props);
	app->run();

	tgui::log::shutdown();

	return EXIT_SUCCESS;
}