#pragma once

#include "tgui/core/base.h"
#include "tgui/platform/window_adapter.h"

#include "tgui/core/reference_object.h"
#include "tgui/core/reference.h"
#include "tgui/core/pixel_position.h"
#include "tgui/core/pixel_size.h"

#include "tgui/renderer/draw_commands.h"
#include "tgui/renderer/texture.h"
#include "tgui/renderer/framebuffer.h"
#include "tgui/renderer/index_buffer.h"
#include "tgui/renderer/vertex_buffer.h"
#include "tgui/renderer/shader.h"

#include "tgui/math/math.h"
#include "tgui/math/color.h"

#include "tgui/core/rotating_buffer.h"

namespace tgui
{
	struct write_flags
	{
		enum Enum : uint16_t
		{
			None = 0,
			R = (1 << 0),
			G = (1 << 1),
			B = (1 << 2),
			A = (1 << 3),
			Z = (1 << 4),
			RGB = R | G | B,
			RGBA = R | G | B | A,
			All = R | G | B | A | Z
		};
	};

	struct primitive_topology
	{
		enum Enum
		{
			Triangles,
			TriangleStrip,
			Lines,
			LineStrip,
			Points
		};
	};

	struct draw_properties
	{
		shader* program = nullptr;
		vertex_buffer* vtx_buf = nullptr;
		index_buffer* idx_buf = nullptr;
		texture* tex = nullptr;

		primitive_topology::Enum topology = primitive_topology::Triangles;
		uint16_t write_flags = write_flags::RGBA;

		uint32_t start_vertex = 0;
		uint32_t vertex_count = 0;

		uint32_t start_index = 0;
		uint32_t index_count = 0;
	};

	class render_interface : public reference_object
	{
	public:
		~render_interface() = default;

		virtual bool init();
		virtual void shutdown();

		virtual void draw(const draw_properties& properties) = 0;
		virtual void resolve(texture* target, framebuffer* source) = 0;

		virtual uint16_t get_pass_index() { return 0; }
		virtual void next_pass(framebuffer* fb = nullptr) {}

	public:
		void resize(const pixel_size& new_size);
		void frame();

		void draw_quad(const glm::vec2& pos, const glm::vec2& size, const color& color);
		void draw_graph(const glm::vec2& pos, const glm::vec2& size, const std::array<glm::vec2, 4>& corners, const color& color);
		void draw_fullscreen_quad(texture* tex);

	protected:
		virtual bool on_init() = 0;
		virtual void on_shutdown() = 0;
		virtual void on_resize(const pixel_size& new_size) = 0;
		virtual void on_frame() = 0;

	protected:
		// shaders
		shader* m_quad_shader = nullptr;
		shader* m_quad_image_shader = nullptr;
		shader* m_fullscreen_shader = nullptr;

		// framebuffers
		framebuffer* m_framebuffer = nullptr;
		texture* m_resolve_texture = nullptr;

		// index buffers
		index_buffer* m_quad_index_buffer = nullptr;

		// vertex buffers
		vertex_buffer* m_quad_vertex_buffer = nullptr;
		vertex_buffer* m_quad_image_vertex_buffer = nullptr;
		vertex_buffer* m_fullscreen_vertex_buffer = nullptr;

		// rotating buffers
		rotating_buffer<quad_vertex> m_quad_vertex_buffer_data;
		rotating_buffer<quad_image_vertex> m_quad_image_vertex_buffer_data;
	};
}