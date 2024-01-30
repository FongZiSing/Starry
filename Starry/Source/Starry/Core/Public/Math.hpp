// Copyright (c) 2024 Fong ZiSing. All rights reserved.
//
//     Math.hpp
//

#pragma once

#include <cstdint>



namespace se::math
{
	/**
	 * @brief Multiples value by itself.
	 */
	template<typename _t>
	[[nodiscard]][[msvc::forceinline]] static constexpr _t square(_t const val) noexcept
	{
		return val * val;
	}

	/**
	 * @brief Cubes the value.
	 */
	template<typename _t>
	[[nodiscard]] [[msvc::forceinline]] static constexpr _t cube(_t const val) noexcept
	{
		return val * val * val;
	}

	/**
	 * @brief Inverts the sign bit conditionally.
	 */
	template<typename _t>
	static void chgsign_if(_t* val, bool condition) noexcept
	{
		if constexpr (sizeof(_t) == 1)
		{
			*(reinterpret_cast<uint8_t*>(val)) ^= (uint8_t(condition) << 7);
		}
		else if constexpr (sizeof(_t) == 2)
		{
			*(reinterpret_cast<uint16_t*>(val)) ^= (uint16_t(condition) << 15);
		}
		else if constexpr (sizeof(_t) == 4)
		{
			*(reinterpret_cast<uint32_t*>(val)) ^= (uint32_t(condition) << 31);
		}
		else if constexpr (sizeof(_t) == 8)
		{
			*(reinterpret_cast<uint64_t*>(val)) ^= (uint64_t(condition) << 63);
		}
	}
}