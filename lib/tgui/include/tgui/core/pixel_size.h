#pragma once

#include "tgui/core/base.h"

namespace tgui
{
	struct pixel_size
	{
		int32_t width;
		int32_t height;

		pixel_size();
		pixel_size(int32_t scalar);
		pixel_size(int32_t w, int32_t h);

		bool operator==(const pixel_size& other) const
		{
			return width == other.width && height == other.height;
		}

		bool operator!=(const pixel_size& other) const
		{
			return !(*this == other);
		}

		// Arithmetic operators
		pixel_size operator+(const pixel_size& other) const
		{
			return pixel_size(width + other.width, height + other.height);
		}

		pixel_size operator-(const pixel_size& other) const
		{
			return pixel_size(width - other.width, height - other.height);
		}

		pixel_size operator*(const pixel_size& other) const
		{
			return pixel_size(width * other.width, height * other.height);
		}

		pixel_size operator/(const pixel_size& other) const
		{
			return pixel_size(width / other.width, height / other.height);
		}

		pixel_size& operator+=(const pixel_size& other)
		{
			width += other.width;
			height += other.height;
			return *this;
		}

		pixel_size& operator-=(const pixel_size& other)
		{
			width -= other.width;
			height -= other.height;
			return *this;
		}

		pixel_size& operator*=(const pixel_size& other)
		{
			width *= other.width;
			height *= other.height;
			return *this;
		}

		pixel_size& operator/=(const pixel_size& other)
		{
			width /= other.width;
			height /= other.height;
			return *this;
		}

		// Scalar operators
		pixel_size operator+(int32_t scalar) const
		{
			return pixel_size(width + scalar, height + scalar);
		}

		pixel_size operator-(int32_t scalar) const
		{
			return pixel_size(width - scalar, height - scalar);
		}

		pixel_size operator*(int32_t scalar) const
		{
			return pixel_size(width * scalar, height * scalar);
		}

		pixel_size& operator+=(int32_t scalar)
		{
			width += scalar;
			height += scalar;
			return *this;
		}

		pixel_size& operator-=(int32_t scalar)
		{
			width -= scalar;
			height -= scalar;
			return *this;
		}

		pixel_size& operator*=(int32_t scalar)
		{
			width *= scalar;
			height *= scalar;
			return *this;
		}

		pixel_size& operator/=(int32_t scalar)
		{
			width /= scalar;
			height /= scalar;
			return *this;
		}
	};
}