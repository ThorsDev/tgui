#pragma once

#include "math.h"

namespace tgui
{
	struct color
	{
		glm::vec4 m_value;

		color() : m_value(1.0f, 0.0f, 0.0f, 1.0f) {}

		color(float r, float g, float b, float a)
			: m_value(r, g, b, a)
		{}

		color(uint32_t color)
			: m_value(float((color >> 16) & 0xFF) / 255.0f, float((color >> 8) & 0xFF) / 255.0f, float((color & 0xFF)) / 255.0f, float((color >> 24) & 0xFF) / 255.0f)
		{}

		color(const glm::vec4& color)
			: m_value(color)
		{}

		color(const glm::vec3& color)
			: m_value(color, 1.0f)
		{}

		color(const char* hex)
		{
			from_hex(hex);
		}

		uint32_t RGBA() const
		{
			return (static_cast<uint32_t>(m_value.a * 255.0f) << 24) | (static_cast<uint32_t>(m_value.r * 255.0f) << 16) | (static_cast<uint32_t>(m_value.g * 255.0f) << 8) | static_cast<uint32_t>(m_value.b * 255.0f);
		}

		uint32_t bgra() const
		{
			return (static_cast<uint32_t>(m_value.a * 255.0f) << 24) | (static_cast<uint32_t>(m_value.b * 255.0f) << 16) | (static_cast<uint32_t>(m_value.g * 255.0f) << 8) | static_cast<uint32_t>(m_value.r * 255.0f);
		}

		uint32_t abgr() const
		{
			return (static_cast<uint32_t>(m_value.r * 255.0f) << 24) | (static_cast<uint32_t>(m_value.g * 255.0f) << 16) | (static_cast<uint32_t>(m_value.b * 255.0f) << 8) | static_cast<uint32_t>(m_value.a * 255.0f);
		}

		uint32_t argb() const
		{
			return (static_cast<uint32_t>(m_value.r * 255.0f) << 24) | (static_cast<uint32_t>(m_value.g * 255.0f) << 16) | (static_cast<uint32_t>(m_value.b * 255.0f) << 8) | static_cast<uint32_t>(m_value.a * 255.0f);
		}

		bool operator==(const color& other) const
		{
			return m_value == other.m_value;
		}

		bool operator!=(const color& other) const
		{
			return !(*this == other);
		}

		color operator+(const color& other) const
		{
			return color(glm::clamp(m_value + other.m_value, 0.0f, 1.0f));
		}

		color& operator+=(const color& other)
		{
			m_value = glm::clamp(m_value + other.m_value, 0.0f, 1.0f);
			return *this;
		}

		color operator-(const color& other) const
		{
			return color(glm::clamp(m_value - other.m_value, 0.0f, 1.0f));
		}

		color& operator-=(const color& other)
		{
			m_value = glm::clamp(m_value - other.m_value, 0.0f, 1.0f);
			return *this;
		}

		color operator*(float scalar) const
		{
			return color(glm::clamp(m_value * scalar, 0.0f, 1.0f));
		}

		color operator*(const color& other) const
		{
			return color(glm::clamp(m_value * other.m_value, 0.0f, 1.0f));
		}

		color& operator*=(float scalar)
		{
			m_value = glm::clamp(m_value * scalar, 0.0f, 1.0f);
			return *this;
		}

		void add(const color& other)
		{
			m_value = glm::clamp(m_value + other.m_value, 0.0f, 1.0f);
		}

		void sub(const color& other)
		{
			m_value = glm::clamp(m_value - other.m_value, 0.0f, 1.0f);
		}

		void mul(float scalar)
		{
			m_value = glm::clamp(m_value * scalar, 0.0f, 1.0f);
		}

		void mul(const color& other)
		{
			m_value = glm::clamp(m_value * other.m_value, 0.0f, 1.0f);
		}

		glm::vec4 to_vec4()
		{
			return m_value;
		}

		glm::vec4 to_vec4_bgra()
		{
			return glm::vec4(m_value.b, m_value.g, m_value.r, m_value.a);
		}

		glm::vec3 to_vec3()
		{
			return glm::vec3(m_value);
		}

		glm::vec3 to_vec3_bgr()
		{
			return glm::vec3(m_value.b, m_value.g, m_value.r);
		}

		float& rR()
		{
			return m_value.r;
		}

		float& g()
		{
			return m_value.g;
		}

		float& b()
		{
			return m_value.b;
		}

		float& a()
		{
			return m_value.a;
		}

		const float r() const
		{
			return m_value.r;
		}

		const float g() const
		{
			return m_value.g;
		}

		const float b() const
		{
			return m_value.b;
		}

		const float a() const
		{
			return m_value.a;
		}

		static color from_hex(const char* hex)
		{
			uint32_t col = 0;
			if (sscanf(hex, "#%x", &col) == 1)
				return color(col);

			return color();
		}

		static color from_hex(uint32_t hex)
		{
			return color(hex);
		}
	};
}