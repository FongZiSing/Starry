// Copyright (c) 2024 Fong ZiSing. All rights reserved.
//
//     Vector.hpp
//

#pragma once

#include <cstdint>
#include "Math.hpp"



namespace se
{
	typedef float real_t;
}



namespace se
{
	struct vec2i_t
	{
		int32_t x, y;


	public:
		constexpr vec2i_t() noexcept : x(0), y(0) {}
		constexpr vec2i_t(int32_t iv) noexcept : x(iv), y(iv) {}
		constexpr vec2i_t(int32_t ix, int32_t iy) noexcept : x(ix), y(iy) {}
		constexpr vec2i_t& operator = (int32_t iv) noexcept { x = y = iv; return *this; }
		constexpr vec2i_t(struct vec2_t const&) noexcept;


	public:
		vec2i_t(vec2i_t const&) = default;
		vec2i_t& operator = (vec2i_t const&) = default;
		vec2i_t(vec2i_t&&) = default;
		vec2i_t& operator = (vec2i_t&&) = default;
		~vec2i_t() noexcept {}


	public:
		int32_t& operator [](int32_t index) noexcept { return (&x)[index]; }
		int32_t const& operator [](int32_t index) const noexcept { return (&x)[index]; }
		bool operator == (vec2i_t const& rhs) const noexcept { return x == rhs.x && y == rhs.y; }
		bool operator != (vec2i_t const& rhs) const noexcept { return x != rhs.x || y != rhs.y; }


	public:
		vec2i_t operator - () const noexcept { return { -x, -y }; }

		vec2i_t operator + (vec2i_t const& rhs) const noexcept { return { x + rhs.x, y + rhs.y }; }
		vec2i_t operator - (vec2i_t const& rhs) const noexcept { return { x - rhs.x, y - rhs.y }; }
		vec2i_t operator * (vec2i_t const& rhs) const noexcept { return { x * rhs.x, y * rhs.y }; }
		vec2i_t operator / (vec2i_t const& rhs) const noexcept { return { x / rhs.x, y / rhs.y }; }
		vec2i_t const& operator += (vec2i_t const& rhs) noexcept { x += rhs.x; y += rhs.y; return *this; }
		vec2i_t const& operator -= (vec2i_t const& rhs) noexcept { x -= rhs.x; y -= rhs.y; return *this; }
		vec2i_t const& operator *= (vec2i_t const& rhs) noexcept { x *= rhs.x; y *= rhs.y; return *this; }
		vec2i_t const& operator /= (vec2i_t const& rhs) noexcept { x /= rhs.x; y /= rhs.y; return *this; }

		vec2i_t operator + (int32_t rhs) const noexcept { return { x + rhs, y + rhs }; }
		vec2i_t operator - (int32_t rhs) const noexcept { return { x - rhs, y - rhs }; }
		vec2i_t operator * (int32_t rhs) const noexcept { return { x * rhs, y * rhs }; }
		vec2i_t operator / (int32_t rhs) const noexcept { return { x / rhs, y / rhs }; }
		vec2i_t const& operator += (int32_t rhs) noexcept { x += rhs; y += rhs; return *this; }
		vec2i_t const& operator -= (int32_t rhs) noexcept { x -= rhs; y -= rhs; return *this; }
		vec2i_t const& operator *= (int32_t rhs) noexcept { x *= rhs; y *= rhs; return *this; }
		vec2i_t const& operator /= (int32_t rhs) noexcept { x /= rhs; y /= rhs; return *this; }
	};
}



namespace se
{
	struct vec2_t
	{
		real_t x, y;


	public:
		constexpr vec2_t() noexcept : x(0), y(0) {}
		constexpr vec2_t(real_t iv) noexcept : x(iv), y(iv) {}
		constexpr vec2_t(real_t ix, real_t iy) noexcept : x(ix), y(iy) {}
		constexpr vec2_t& operator = (real_t iv) noexcept { x = y = iv; return *this; }
		constexpr vec2_t(struct vec2i_t const&) noexcept;


	public:
		real_t& operator [](int32_t index) noexcept { return (&x)[index]; }
		real_t const& operator [](int32_t index) const noexcept { return (&x)[index]; }
		bool operator == (vec2_t const& rhs) const noexcept { return x == rhs.x && y == rhs.y; }
		bool operator != (vec2_t const& rhs) const noexcept { return x != rhs.x || y != rhs.y; }


	public:
		vec2_t operator - () const noexcept { return { -x, -y }; }

		vec2_t operator + (vec2_t const& rhs) const noexcept { return { x + rhs.x, y + rhs.y }; }
		vec2_t operator - (vec2_t const& rhs) const noexcept { return { x - rhs.x, y - rhs.y }; }
		vec2_t operator * (vec2_t const& rhs) const noexcept { return { x * rhs.x, y * rhs.y }; }
		vec2_t operator / (vec2_t const& rhs) const noexcept { return { x / rhs.x, y / rhs.y }; }
		vec2_t const& operator += (vec2_t const& rhs) noexcept { x += rhs.x; y += rhs.y; return *this; }
		vec2_t const& operator -= (vec2_t const& rhs) noexcept { x -= rhs.x; y -= rhs.y; return *this; }
		vec2_t const& operator *= (vec2_t const& rhs) noexcept { x *= rhs.x; y *= rhs.y; return *this; }
		vec2_t const& operator /= (vec2_t const& rhs) noexcept { x /= rhs.x; y /= rhs.y; return *this; }

		vec2_t operator + (real_t rhs) const noexcept { return { x + rhs, y + rhs }; }
		vec2_t operator - (real_t rhs) const noexcept { return { x - rhs, y - rhs }; }
		vec2_t operator * (real_t rhs) const noexcept { return { x * rhs, y * rhs }; }
		vec2_t operator / (real_t rhs) const noexcept { return { x / rhs, y / rhs }; }
		vec2_t const& operator += (real_t rhs) noexcept { x += rhs; y += rhs; return *this; }
		vec2_t const& operator -= (real_t rhs) noexcept { x -= rhs; y -= rhs; return *this; }
		vec2_t const& operator *= (real_t rhs) noexcept { x *= rhs; y *= rhs; return *this; }
		vec2_t const& operator /= (real_t rhs) noexcept { x /= rhs; y /= rhs; return *this; }


	public:
		real_t length_squared() const noexcept { return math::square(x) + math::square(y); }
		real_t distance_squared(vec2_t const& v) const noexcept { return math::square(x - v.x) + math::square(y - v.y); }
	};



	constexpr vec2i_t::vec2i_t(vec2_t const& iv) noexcept
		: x{ static_cast<int32_t>(iv.x) }
		, y{ static_cast<int32_t>(iv.y) }
	{}

	constexpr vec2_t::vec2_t(vec2i_t const& iv) noexcept
		: x{ static_cast<real_t>(iv.x) }
		, y{ static_cast<real_t>(iv.y) }
	{}
}



inline se::vec2i_t operator + (int32_t lhs, se::vec2i_t const& rhs) noexcept { return { lhs + rhs.x, lhs + rhs.y }; }
inline se::vec2i_t operator - (int32_t lhs, se::vec2i_t const& rhs) noexcept { return { lhs - rhs.x, lhs - rhs.y }; }
inline se::vec2i_t operator * (int32_t lhs, se::vec2i_t const& rhs) noexcept { return { lhs * rhs.x, lhs * rhs.y }; }
inline se::vec2i_t operator / (int32_t lhs, se::vec2i_t const& rhs) noexcept { return { lhs / rhs.x, lhs / rhs.y }; }

inline se::vec2_t operator + (se::real_t lhs, se::vec2_t const& rhs) noexcept { return { lhs + rhs.x, lhs + rhs.y }; }
inline se::vec2_t operator - (se::real_t lhs, se::vec2_t const& rhs) noexcept { return { lhs - rhs.x, lhs - rhs.y }; }
inline se::vec2_t operator * (se::real_t lhs, se::vec2_t const& rhs) noexcept { return { lhs * rhs.x, lhs * rhs.y }; }
inline se::vec2_t operator / (se::real_t lhs, se::vec2_t const& rhs) noexcept { return { lhs / rhs.x, lhs / rhs.y }; }



namespace se
{
	struct vec4_t
	{
	public:
		real_t x, y, z, w;


	public:
		constexpr vec4_t() noexcept : x(0), y(0), z(0), w(0) {}
		constexpr vec4_t(real_t v) noexcept : x(v), y(v), z(v), w(v) {}
		constexpr vec4_t(real_t ix, real_t iy, real_t iz, real_t iw) noexcept : x(ix), y(iy), z(iz), w(iw) {}
		constexpr vec4_t& operator = (real_t iv) noexcept { x = y = z = w = iv; return *this; }
	};
}