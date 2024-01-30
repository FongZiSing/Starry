// Copyright (c) 2024 Fong ZiSing. All rights reserved.
//
//     Vector.hpp
//

#pragma once

#include "Math.hpp"

#include <cstdint>



namespace se
{
	struct vec2i
	{
		int32_t x, y;


	public:
		constexpr vec2i() noexcept : x(0), y(0) {}
		constexpr vec2i(int32_t iv) noexcept : x(iv), y(iv) {}
		constexpr vec2i(int32_t ix, int32_t iy) noexcept : x(ix), y(iy) {}
		constexpr vec2i& operator = (int32_t iv) noexcept { x = y = iv; return *this; }
		constexpr vec2i(struct vec2 const&) noexcept;


	public:
		vec2i(vec2i const&) = default;
		vec2i& operator = (vec2i const&) = default;
		vec2i(vec2i&&) = default;
		vec2i& operator = (vec2i&&) = default;
		~vec2i() noexcept {}


	public:
		int32_t& operator [](int32_t index) noexcept { return (&x)[index]; }
		int32_t const& operator [](int32_t index) const noexcept { return (&x)[index]; }
		bool operator == (vec2i const& rhs) const noexcept { return x == rhs.x && y == rhs.y; }
		bool operator != (vec2i const& rhs) const noexcept { return x != rhs.x || y != rhs.y; }


	public:
		vec2i operator - () const noexcept { return { -x, -y }; }

		vec2i operator + (vec2i const& rhs) const noexcept { return { x + rhs.x, y + rhs.y }; }
		vec2i operator - (vec2i const& rhs) const noexcept { return { x - rhs.x, y - rhs.y }; }
		vec2i operator * (vec2i const& rhs) const noexcept { return { x * rhs.x, y * rhs.y }; }
		vec2i operator / (vec2i const& rhs) const noexcept { return { x / rhs.x, y / rhs.y }; }
		vec2i const& operator += (vec2i const& rhs) noexcept { x += rhs.x; y += rhs.y; return *this; }
		vec2i const& operator -= (vec2i const& rhs) noexcept { x -= rhs.x; y -= rhs.y; return *this; }
		vec2i const& operator *= (vec2i const& rhs) noexcept { x *= rhs.x; y *= rhs.y; return *this; }
		vec2i const& operator /= (vec2i const& rhs) noexcept { x /= rhs.x; y /= rhs.y; return *this; }

		vec2i operator + (int32_t rhs) const noexcept { return { x + rhs, y + rhs }; }
		vec2i operator - (int32_t rhs) const noexcept { return { x - rhs, y - rhs }; }
		vec2i operator * (int32_t rhs) const noexcept { return { x * rhs, y * rhs }; }
		vec2i operator / (int32_t rhs) const noexcept { return { x / rhs, y / rhs }; }
		vec2i const& operator += (int32_t rhs) noexcept { x += rhs; y += rhs; return *this; }
		vec2i const& operator -= (int32_t rhs) noexcept { x -= rhs; y -= rhs; return *this; }
		vec2i const& operator *= (int32_t rhs) noexcept { x *= rhs; y *= rhs; return *this; }
		vec2i const& operator /= (int32_t rhs) noexcept { x /= rhs; y /= rhs; return *this; }
	};
}



namespace se
{
	struct vec2
	{
		float x, y;


	public:
		constexpr vec2() noexcept : x(0), y(0) {}
		constexpr vec2(float iv) noexcept : x(iv), y(iv) {}
		constexpr vec2(float ix, float iy) noexcept : x(ix), y(iy) {}
		constexpr vec2& operator = (float iv) noexcept { x = y = iv; return *this; }
		constexpr vec2(struct vec2i const&) noexcept;


	public:
		float& operator [](int32_t index) noexcept { return (&x)[index]; }
		float const& operator [](int32_t index) const noexcept { return (&x)[index]; }
		bool operator == (vec2 const& rhs) const noexcept { return x == rhs.x && y == rhs.y; }
		bool operator != (vec2 const& rhs) const noexcept { return x != rhs.x || y != rhs.y; }


	public:
		vec2 operator - () const noexcept { return { -x, -y }; }

		vec2 operator + (vec2 const& rhs) const noexcept { return { x + rhs.x, y + rhs.y }; }
		vec2 operator - (vec2 const& rhs) const noexcept { return { x - rhs.x, y - rhs.y }; }
		vec2 operator * (vec2 const& rhs) const noexcept { return { x * rhs.x, y * rhs.y }; }
		vec2 operator / (vec2 const& rhs) const noexcept { return { x / rhs.x, y / rhs.y }; }
		vec2 const& operator += (vec2 const& rhs) noexcept { x += rhs.x; y += rhs.y; return *this; }
		vec2 const& operator -= (vec2 const& rhs) noexcept { x -= rhs.x; y -= rhs.y; return *this; }
		vec2 const& operator *= (vec2 const& rhs) noexcept { x *= rhs.x; y *= rhs.y; return *this; }
		vec2 const& operator /= (vec2 const& rhs) noexcept { x /= rhs.x; y /= rhs.y; return *this; }

		vec2 operator + (float rhs) const noexcept { return { x + rhs, y + rhs }; }
		vec2 operator - (float rhs) const noexcept { return { x - rhs, y - rhs }; }
		vec2 operator * (float rhs) const noexcept { return { x * rhs, y * rhs }; }
		vec2 operator / (float rhs) const noexcept { return { x / rhs, y / rhs }; }
		vec2 const& operator += (float rhs) noexcept { x += rhs; y += rhs; return *this; }
		vec2 const& operator -= (float rhs) noexcept { x -= rhs; y -= rhs; return *this; }
		vec2 const& operator *= (float rhs) noexcept { x *= rhs; y *= rhs; return *this; }
		vec2 const& operator /= (float rhs) noexcept { x /= rhs; y /= rhs; return *this; }


	public:
		float length_squared() const noexcept { return math::square(x) + math::square(y); }
		float distance_squared(vec2 const& v) const noexcept { return math::square(x - v.x) + math::square(y - v.y); }
	};



	constexpr vec2i::vec2i(vec2 const& iv) noexcept
		: x{ static_cast<int32_t>(iv.x) }
		, y{ static_cast<int32_t>(iv.y) }
	{}

	constexpr vec2::vec2(vec2i const& iv) noexcept
		: x{ static_cast<float>(iv.x) }
		, y{ static_cast<float>(iv.y) }
	{}
}



inline se::vec2i operator + (int32_t lhs, se::vec2i const& rhs) noexcept { return { lhs + rhs.x, lhs + rhs.y }; }
inline se::vec2i operator - (int32_t lhs, se::vec2i const& rhs) noexcept { return { lhs - rhs.x, lhs - rhs.y }; }
inline se::vec2i operator * (int32_t lhs, se::vec2i const& rhs) noexcept { return { lhs * rhs.x, lhs * rhs.y }; }
inline se::vec2i operator / (int32_t lhs, se::vec2i const& rhs) noexcept { return { lhs / rhs.x, lhs / rhs.y }; }

inline se::vec2 operator + (float lhs, se::vec2 const& rhs) noexcept { return { lhs + rhs.x, lhs + rhs.y }; }
inline se::vec2 operator - (float lhs, se::vec2 const& rhs) noexcept { return { lhs - rhs.x, lhs - rhs.y }; }
inline se::vec2 operator * (float lhs, se::vec2 const& rhs) noexcept { return { lhs * rhs.x, lhs * rhs.y }; }
inline se::vec2 operator / (float lhs, se::vec2 const& rhs) noexcept { return { lhs / rhs.x, lhs / rhs.y }; }