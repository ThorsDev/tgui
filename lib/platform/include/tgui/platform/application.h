#pragma once

#include "tgui/platform/window.h"
#include "tgui/core/assert.h"
#include "tgui/core/timestep.h"

#include "tgui/event/window_events.h"

namespace tgui::platform
{
	class application : public tgui::event_listener
	{
	public:
		struct properties
		{
			window_adapter::properties window_properties;

			// ...
		};

	public:
		application(const application::properties& props);
		~application();

		void run();

		void update();
		void render();

		tgui::platform::window* get_window() const;
		
		void close();

		timestep get_timestep() const;
		timestep get_frametime() const;
		float get_time() const;

	private:
		void init();
		void shutdown();

		void event_callback(event& e);

	protected:
		virtual void on_init() {};
		virtual void on_shutdown() {};

		virtual void on_update(timestep ts){}
		virtual void on_render(timestep ts){}

	protected:
		void on_window_close(window_close_event& e);

	private:
		application::properties m_properties = {};
		std::unique_ptr<tgui::platform::window> m_window = nullptr;

		bool m_should_close = false;

		timestep m_frame_time;
		timestep m_time_step;
		float m_last_frame_time = 0.0f;

	public:
		inline static application& get() { 
			TGUI_ASSERT(ms_instance, "application instance doesn't exist yet");
			return *ms_instance; 
		}

	private:
		inline static application* ms_instance = nullptr;
	};
}