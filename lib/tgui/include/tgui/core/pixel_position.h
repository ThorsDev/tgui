#pragma once

#include "tgui/core/base.h"

namespace tgui
{
	struct pixel_position
	{
		int32_t x;
		int32_t y;

		pixel_position();
		pixel_position(int32_t scalar);
		pixel_position(int32_t x_, int32_t y_);

		bool operator==(const pixel_position& other) const
		{
			return x == other.x && y == other.y;
		}

		bool operator!=(const pixel_position& other) const
		{
			return !(*this == other);
		}

		// Arithmetic operators
		pixel_position operator+(const pixel_position& other) const
		{
			return pixel_position(x + other.x, y + other.y);
		}

		pixel_position operator-(const pixel_position& other) const
		{
			return pixel_position(x - other.x, y - other.y);
		}

		pixel_position operator*(const pixel_position& other) const
		{
			return pixel_position(x * other.x, y * other.y);
		}

		pixel_position operator/(const pixel_position& other) const
		{
			return pixel_position(x / other.x, y / other.y);
		}

		pixel_position& operator+=(const pixel_position& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}

		pixel_position& operator-=(const pixel_position& other)
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}

		pixel_position& operator*=(const pixel_position& other)
		{
			x *= other.x;
			y *= other.y;
			return *this;
		}

		pixel_position& operator/=(const pixel_position& other)
		{
			x /= other.x;
			y /= other.y;
			return *this;
		}

		// Scalar operators
		pixel_position operator+(int32_t scalar) const
		{
			return pixel_position(x + scalar, y + scalar);
		}

		pixel_position operator-(int32_t scalar) const
		{
			return pixel_position(x - scalar, y - scalar);
		}

		pixel_position operator*(int32_t scalar) const
		{
			return pixel_position(x * scalar, y * scalar);
		}

		pixel_position& operator+=(int32_t scalar)
		{
			x += scalar;
			y += scalar;
			return *this;
		}

		pixel_position& operator-=(int32_t scalar)
		{
			x -= scalar;
			y -= scalar;
			return *this;
		}

		pixel_position& operator*=(int32_t scalar)
		{
			x *= scalar;
			y *= scalar;
			return *this;
		}

		pixel_position& operator/=(int32_t scalar)
		{
			x /= scalar;
			y /= scalar;
			return *this;
		}
	};
}