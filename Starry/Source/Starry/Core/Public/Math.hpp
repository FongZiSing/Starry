// Copyright (c) 2024 Fong ZiSing. All rights reserved.
//
//     Math.hpp
//

#pragma once



namespace se::math
{
	/**
	 * @brief Multiples value by itself.
	 */
	template< class T >
	[[nodiscard]][[msvc::forceinline]] static constexpr T square(T const val) noexcept
	{
		return val * val;
	}

	/**
	 * @brief Cubes the value.
	 */
	template< class T >
	[[nodiscard]] [[msvc::forceinline]] static constexpr T cube(T const val) noexcept
	{
		return val * val * val;
	}
}