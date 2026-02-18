#pragma once

#include <tgui/core/base.h>
#include <tgui/event/event.h>

#include <tgui/platform/window_adapter.h>

namespace tgui::platform
{
	class window : public window_adapter
	{
	public:
		using native_window_handle = void*;
		using event_callback_fn = std::function<void(event&)>;

	public:
		window(const window_adapter::properties& props);
		virtual ~window();

		void process_events();
		void set_event_callback(const event_callback_fn& cb);

		void on_event(event& ev);

	public:
		virtual void set_mouse_pos(const pixel_position& pos) override;
		virtual pixel_position get_mouse_pos() const override;

		virtual void set_size(const pixel_size& size) override;
		virtual pixel_size get_size() const override;

		virtual void set_position(const pixel_position& pos) override;
		virtual pixel_position get_position() const override;

		void set_title(const std::string& title) override;
		const std::string& get_title() const override;

		bool is_minimized() const override;
		bool is_maximized() const override;

		void show() override;
		void hide() override;

	public:
		void* get_internal_window() const;
		window::native_window_handle get_native_window() const;

	private:
		void init();
		void shutdown();

	private:
		void* m_window = nullptr;

		event_callback_fn m_event_callback;
	};
}