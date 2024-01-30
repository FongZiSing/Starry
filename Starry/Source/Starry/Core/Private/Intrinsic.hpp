// Copyright (c) 2024 Fong ZiSing. All rights reserved.
//
//     Intrinsic.cpp
//

#pragma once

#include <cstdint>
#include <intrin.h>



namespace se
{
	using float4 = __m128;

	namespace constants4f
	{
		static constexpr float4 sign_bit{ .m128_f32 = { -0.f, -0.f, -0.f, -0.f } };
		static constexpr float4 sign_mask{ .m128_u32 = { 0x7fffffffu, 0x7fffffffu, 0x7fffffffu, 0x7fffffffu } };
		static constexpr float4 non_fractional{ .m128_f32 = { 8388608.f, 8388608.f, 8388608.f, 8388608.f } };
	}

	[[nodiscard]][[msvc::forceinline]] static float4 zero4f() noexcept
	{
		return _mm_setzero_ps();
	}

	[[nodiscard]][[msvc::forceinline]] static float4 make(float v) noexcept
	{
		return _mm_set_ps1(v);
	}

	[[nodiscard]][[msvc::forceinline]] static float4 make(float x, float y, float z, float w) noexcept
	{
		return _mm_setr_ps(x, y, z, w);
	}

	[[nodiscard]][[msvc::forceinline]] static float4 load(const void* ptr) noexcept
	{
		return _mm_loadu_ps((const float*)ptr);
	}

	[[msvc::forceinline]] static void store(float4 const& xmm, float* to) noexcept
	{
		_mm_storeu_ps(to, xmm);
	}

	[[nodiscard]][[msvc::forceinline]] static float4 load_aligned(const float* ptr) noexcept
	{
		return _mm_load_ps(ptr);
	}

	[[msvc::forceinline]] static void store_aligned(float4 const& xmm, float* to) noexcept
	{
		_mm_store_ps(to, xmm);
	}

	template<int i>
	[[nodiscard]][[msvc::forceinline]] static float extract(float4 const& xmm) noexcept
	{
		return _mm_extract_ps(xmm, i);
	}
	
	/**
	 * @brief Does a bitwise selection based on a mask.
	 * @return float4( (mask.x & 0x80000000) ? xmm1.x : xmm2.x, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 select(float4 const& mask, float4 const& xmm1, float4 const& xmm2) noexcept
	{
		return _mm_blendv_ps(xmm2, xmm1, mask);
	}

	/**
	 * @brief Selects two elements via a shuffle mask.
	 * @returns float4( xmm1[i], xmm1[j], xmm2[k], xmm2[l] )
	 */
	template<int i, int j, int k, int l>
	[[nodiscard]][[msvc::forceinline]] static float4 shuffle(float4 const& xmm1, float4 const& xmm2) noexcept
	{
		constexpr int mask = i | (j << 2) | (k << 4) | (l << 6);
		return _mm_shuffle_ps(xmm1, xmm2, mask);
	}

	/**
	 * @brief Swizzles the elements.
	 * @returns float4( xmm[i], xmm[j], xmm[k], xmm[l] )
	 */
	template<int i, int j, int k, int l>
	[[nodiscard]][[msvc::forceinline]] static float4 swizzle(float4 const& xmm) noexcept
	{
		constexpr int mask = i | (j << 2) | (k << 4) | (l << 6);
		return _mm_shuffle_ps(xmm, xmm, mask);
	}

	/**
	 * @brief Replicates one element into all four elements.
	 * @returns float4( xmm[i], xmm[i], xmm[i], xmm[i] )
	 */
	template<int i>
	[[nodiscard]][[msvc::forceinline]] static float4 replicate(float4 const& xmm) noexcept
	{
		constexpr int mask = i | (i << 2) | (i << 4) | (i << 6);
		return _mm_shuffle_ps(xmm, xmm, mask);
	}
	
	/**
	 * @brief Computes the logical EQUAL for each elements.
	 * @return float4 ( xmm1.x == xmm2.x ? 0xFFFFFFFF : 0, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 eq(float4 const& xmm1, float4 const& xmm2) noexcept
	{
		return _mm_cmpeq_ps(xmm1, xmm2);
	}

	/**
	 * @brief Computes the logical NOT EQUAL for each elements.
	 * @return float4 ( xmm1.x != xmm2.x ? 0xFFFFFFFF : 0, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 ne(float4 const& xmm1, float4 const& xmm2) noexcept
	{
		return _mm_cmpneq_ps(xmm1, xmm2);
	}

	/**
	 * @brief Computes the logical GREATER THAN for each elements.
	 * @return float4 ( xmm1.x > xmm2.x ? 0xFFFFFFFF : 0, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 gt(float4 const& xmm1, float4 const& xmm2) noexcept
	{
		return _mm_cmpgt_ps(xmm1, xmm2);
	}

	/**
	 * @brief Computes the logical LESS THAN for each elements.
	 * @return float4 ( xmm1.x < xmm2.x ? 0xFFFFFFFF : 0, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 lt(float4 const& xmm1, float4 const& xmm2) noexcept
	{
		return _mm_cmplt_ps(xmm1, xmm2);
	}

	/**
	 * @brief Computes the logical GREATER THAN OR EQUAL for each elements.
	 * @return float4 ( xmm1.x >= xmm2.x ? 0xFFFFFFFF : 0, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 ge(float4 const& xmm1, float4 const& xmm2) noexcept
	{
		return _mm_cmpge_ps(xmm1, xmm2);
	}

	/**
	 * @brief Computes the logical LESS THAN OR EQUAL for each elements.
	 * @return float4 ( xmm1.x <= xmm2.x ? 0xFFFFFFFF : 0, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 le(float4 const& xmm1, float4 const& xmm2) noexcept
	{
		return _mm_cmple_ps(xmm1, xmm2);
	}

	/**
	 * @brief Computes the negation for each elements.
	 * @return float4 ( - xmm2.x, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 neg(float4 const& xmm) noexcept
	{
		return _mm_sub_ps(_mm_setzero_ps(), xmm);
	}

	/**
	 * @brief Computes the addition for each elements.
	 * @return float4 ( xmm1.x + xmm2.x, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 add(float4 const& xmm1, float4 const& xmm2) noexcept
	{
		return _mm_add_ps(xmm1, xmm2);
	}

	/**
	 * @brief Computes the subtraction for each elements.
	 * @return float4 ( xmm1.x - xmm2.x, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 sub(float4 const& xmm1, float4 const& xmm2) noexcept
	{
		return _mm_sub_ps(xmm1, xmm2);
	}

	/**
	 * @brief Computes the multiplication for each elements.
	 * @return float4 ( xmm1.x * xmm2.x, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 mul(float4 const& xmm1, float4 const& xmm2) noexcept
	{
		return _mm_mul_ps(xmm1, xmm2);
	}

	/**
	 * @brief Computes the division for each elements.
	 * @return float4 ( xmm1.x / xmm2.x, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 div(float4 const& xmm1, float4 const& xmm2) noexcept
	{
		return _mm_sub_ps(xmm1, xmm2);
	}

	/**
	 * @brief Computes the module for each elements.
	 * @return float4 ( xmm1.x % xmm2.x, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 mod(float4 const& xmm1, float4 const& xmm2) noexcept
	{
		float4 div = _mm_div_ps(xmm1, xmm2);

		// Floats where abs(f) >= 2^23 have no fractional portion, and larger values would overflow RegisterTruncate.
		float4 no_fraction_mask = _mm_cmpge_ps(_mm_and_ps(div, constants4f::sign_mask), constants4f::non_fractional);
		float4 temp = _mm_blendv_ps(_mm_cvtepi32_ps(_mm_cvttps_epi32(div)), div, no_fraction_mask);
		float4 result = _mm_sub_ps(xmm1, _mm_mul_ps(xmm2, temp));

		// Clamp to [-abs2, abs2] because of possible failures for very large numbers (>1e10) due to precision loss.
		float4 abs2 = _mm_and_ps(xmm2, constants4f::sign_mask);
		return _mm_max_ps(_mm_sub_ps(_mm_setzero_ps(), abs2), _mm_min_ps(result, abs2));
	}

	/**
	 * @brief Computes the multiplication and addition for each elements.
	 * @return float4 ( xmm1.x * xmm2.x + xmm3.x, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 mul_add(float4 const& xmm1, float4 const& xmm2, float4 const& xmm3) noexcept
	{
		return _mm_fmadd_ps(xmm1, xmm2, xmm3);
	}

	/**
	 * @brief Computes the multiplication and addition for each elements.
	 * @return float4 ( xmm1.x * xmm2.x + xmm3.x * xmm4.x, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 mul_add(float4 const& xmm1, float4 const& xmm2, float4 const& xmm3, float4 const& xmm4) noexcept
	{
		return _mm_fmadd_ps(xmm4, xmm3, _mm_mul_ps(xmm2, xmm1));
	}

	/**
	 * @brief Computes the multiplication and subtraction for each elements.
	 * @return float4 ( xmm1.x * xmm2.x - xmm3.x, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 mul_sub(float4 const& xmm1, float4 const& xmm2, float4 const& xmm3) noexcept
	{
		return _mm_fmsub_ps(xmm1, xmm2, xmm3);
	}

	/**
	 * @brief Computes the multiplication and addition for each elements.
	 * @return float4 ( - (xmm1.x * xmm2.x) + xmm3.x, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 nmul_add(float4 const& xmm1, float4 const& xmm2, float4 const& xmm3) noexcept
	{
		return _mm_fnmadd_ps(xmm1, xmm2, xmm3);
	}

	/**
	 * @brief Computes the multiplication and subtraction for each elements.
	 * @return float4 ( - (xmm1.x * xmm2.x) - xmm3.x, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 nmul_sub(float4 const& xmm1, float4 const& xmm2, float4 const& xmm3) noexcept
	{
		return _mm_fnmsub_ps(xmm1, xmm2, xmm3);
	}

	/**
	 * @brief Computes the `min()` for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 min(float4 const& xmm1, float4 const& xmm2) noexcept
	{
		return _mm_min_ps(xmm2, xmm1);
	}

	/**
	 * @brief Computes the `min()` for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 min(float4 const& xmm1, float4 const& xmm2, float4 const& xmm3) noexcept
	{
		return _mm_min_ps(_mm_min_ps(xmm3, xmm2), xmm1);
	}

	/**
	 * @brief Computes the `max()` for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 max(float4 const& xmm1, float4 const& xmm2) noexcept
	{
		return _mm_max_ps(xmm2, xmm1);
	}

	/**
	 * @brief Computes the `max()` for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 max(float4 const& xmm1, float4 const& xmm2, float4 const& xmm3) noexcept
	{
		return _mm_max_ps(_mm_max_ps(xmm3, xmm2), xmm1);
	}

	/**
	 * @brief Computes the `sqrt()` for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 sqrt(float4 const& xmm) noexcept
	{
		return _mm_sqrt_ps(xmm);
	}

	/**
	 * @brief Computes the `1/sqrt()` for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 recip_sqrt(float4 const& xmm) noexcept
	{
		return _mm_rsqrt_ps(xmm);
	}

	/**
	 * @brief Computes the squares for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 square(float4 const& xmm) noexcept
	{
		return _mm_mul_ps(xmm, xmm);
	}

	/**
	 * @brief Computes the cubes for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 cube(float4 const& xmm) noexcept
	{
		return _mm_mul_ps(_mm_mul_ps(xmm, xmm), xmm);
	}

	/**
	 * @brief Computes the `pow()` for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 pow(float4 const& xmm1, float4 const& xmm2) noexcept
	{
		return _mm_pow_ps(xmm1, xmm2);
	}

	/**
	 * @brief Computes the approximations of reciprocals for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 reciprocal(float4 const& xmm) noexcept
	{
		return _mm_rcp_ps(xmm);
	}

	/**
	 * @brief Calculates the `abs()` for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 abs(float4 const& xmm) noexcept
	{
		return _mm_and_ps(constants4f::sign_mask, xmm);
	}

	/**
	 * @brief Rounds `xmm` to an integer with truncation towards zero for each elements.
	 *        (e.g. -1.7 truncated to -1, 2.8 truncated to 2)
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 truncate(float4 const& xmm) noexcept
	{
		return _mm_round_ps(xmm, _MM_FROUND_TRUNC); // SSE4.1
	}

	/**
	 * @brief Calculates the `round()` for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 round(float4 const& xmm) noexcept
	{
		return _mm_round_ps(xmm, _MM_FROUND_NINT); // SSE4.1
	}

	/**
	 * @brief Calculates the `floor()` for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 floor(float4 const& xmm) noexcept
	{
		float4 trunc = _mm_cvtepi32_ps(_mm_cvttps_epi32(xmm));
		float4 mask = _mm_cmpge_ps(xmm, _mm_setzero_ps());
		float4 sub = _mm_blendv_ps(_mm_set_ps1(1), _mm_setzero_ps(), mask);
		return _mm_sub_ps(trunc, sub);
	}

	/**
	 * @brief Calculates the `ceil()` for each elements. 
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 ceil(float4 const& xmm) noexcept
	{
		__m128 trunc = _mm_cvtepi32_ps(_mm_cvttps_epi32(xmm));
		__m128 mask = _mm_cmpge_ps(xmm, _mm_setzero_ps());
		__m128 add = _mm_blendv_ps(_mm_setzero_ps(), _mm_set_ps1(1), mask);
		return _mm_add_ps(trunc, add);
	}

	/**
	 * @brief Returns the length of the vector `xmm`.
	 */
	[[nodiscard]][[msvc::forceinline]] static float magnitude(float4 const& xmm) noexcept
	{
		return _mm_cvtss_f32(_mm_dp_ps(xmm, xmm, 0xf1)); // SSE4.1
	}

	/**
	 * @brief Dot product.
	 */
	[[nodiscard]][[msvc::forceinline]] static float dot_product(float4 const& xmm1, float4 const& xmm2) noexcept
	{
		return _mm_cvtss_f32(_mm_dp_ps(xmm1, xmm2, 0xf1)); // SSE4.1
	}

	/**
	 * @brief Calculates the bitwise AND for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 bit_and(float4 const& xmm1, float4 const& xmm2) noexcept
	{
		return _mm_and_ps(xmm1, xmm2);
	}

	/**
	 * @brief Calculates the bitwise AND-NOT for each elements.
	 * @returns float4( ( ~xmm1.x ) & xmm2.x, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 bit_addnot(float4 const& xmm1, float4 const& xmm2) noexcept
	{
		return _mm_andnot_ps(xmm1, xmm2);
	}

	/**
	 * @brief Calculates the bitwise OR for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 bit_or(float4 const& xmm1, float4 const& xmm2) noexcept
	{
		return _mm_or_ps(xmm1, xmm2);
	}

	/**
	 * @brief Calculates the bitwise XOR for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 bit_xor(float4 const& xmm1, float4 const& xmm2) noexcept
	{
		return _mm_xor_ps(xmm1, xmm2);
	}

	/**
	 * @brief Returns an integer bit-mask (0x00 - 0x0f) based on the sign-bit for each elements.
	 * @return Bit 0 = sign(xmm.x), Bit 1 = sign(xmm.y), Bit 2 = sign(xmm.z), Bit 3 = sign(xmm.w)
	 */
	[[nodiscard]][[msvc::forceinline]] static int sign_masks(float4 const& xmm) noexcept
	{
		return _mm_movemask_ps(xmm);
	}

	/**
	 * @brief Returns an integer bit-mask (0x00 - 0x0f) based on the sign-bit for each elements.
	 * @return Bit 0 = sign(xmm.x), Bit 1 = sign(xmm.y), Bit 2 = sign(xmm.z), Bit 3 = sign(xmm.w)
	 */
	[[nodiscard]][[msvc::forceinline]] static float4 copy_sign(float4 const& xmm, float4 const& sign) noexcept
	{
		return _mm_or_ps(_mm_and_ps(constants4f::sign_bit, sign), _mm_andnot_ps(constants4f::sign_bit, xmm));
	}
}



namespace se
{
	using int4 = __m128i;

	namespace constants4i
	{
		static constexpr int4 sign_bit{ .m128i_u32 = { 0x80000000u, 0x80000000u, 0x80000000u, 0x80000000u } };
		static constexpr int4 sign_mask{ .m128i_u32 = { 0x7fffffffu, 0x7fffffffu, 0x7fffffffu, 0x7fffffffu } };
		static constexpr int4 elem_mask{ .m128i_u32 = { 0xffffffffu, 0xffffffffu, 0xffffffffu, 0xffffffffu } };
	}

	[[nodiscard]][[msvc::forceinline]] static int4 zero4i() noexcept
	{
		return _mm_setzero_si128();
	}

	[[nodiscard]][[msvc::forceinline]] static int4 make(int v) noexcept
	{
		return _mm_set1_epi32(v);
	}

	[[nodiscard]][[msvc::forceinline]] static int4 make(int x, int y, int z, int w) noexcept
	{
		return _mm_setr_epi32(x, y, z, w);
	}

	[[nodiscard]][[msvc::forceinline]] static int4 load(const int* ptr) noexcept
	{
		return _mm_load_si128((const int4*)ptr);
	}

	[[msvc::forceinline]] static void store(int4 const& xmm, int* to) noexcept
	{
		_mm_store_si128((int4*)to, xmm);
	}

	[[nodiscard]][[msvc::forceinline]] static int4 load_aligned(const int* ptr) noexcept
	{
		return _mm_loadu_si128((const int4*)ptr);
	}

	[[msvc::forceinline]] static void store_aligned(int4 const& xmm, float* to) noexcept
	{
		_mm_storeu_si128((int4*)to, xmm);
	}

	template<int i>
	[[msvc::forceinline]] static int extract(int4 const& xmm) noexcept
	{
		return _mm_extract_epi32(xmm, i);
	}

	/**
	 * @brief Selects two elements via a shuffle mask.
	 * @returns int4( xmm[i], xmm[j], xmm[k], xmm[l] )
	 */
	template<int i, int j, int k, int l>
	[[nodiscard]][[msvc::forceinline]] static int4 swizzle(int4 const& xmm) noexcept
	{
		constexpr int mask = i | (j << 2) | (k << 4) | (l << 6);
		return _mm_shuffle_epi32(xmm, mask);
	}

	/**
	 * @brief Replicates one element into all four elements.
	 * @returns int4( xmm[i], xmm[i], xmm[i], xmm[i] )
	 */
	template<int i>
	[[nodiscard]][[msvc::forceinline]] static int4 replicate(int4 const& xmm) noexcept
	{
		constexpr int mask = i | (i << 2) | (i << 4) | (i << 6);
		return _mm_shuffle_epi32(xmm, mask);
	}

	/**
	 * @brief Computes the logical EQUAL for each elements.
	 * @return int4 ( xmm1.x == xmm2.x ? 0xFFFFFFFF : 0, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static int4 eq(int4 const& xmm1, int4 const& xmm2) noexcept
	{
		return _mm_cmpeq_epi32(xmm1, xmm2);
	}

	/**
	 * @brief Computes the logical NOT EQUAL for each elements.
	 * @return int4 ( xmm1.x != xmm2.x ? 0xFFFFFFFF : 0, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static int4 ne(int4 const& xmm1, int4 const& xmm2) noexcept
	{
		return _mm_xor_si128(_mm_cmpeq_epi32(xmm1, xmm2), constants4i::elem_mask);
	}

	/**
	 * @brief Computes the logical GREATER THAN for each elements.
	 * @return int4 ( xmm1.x > xmm2.x ? 0xFFFFFFFF : 0, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static int4 gt(int4 const& xmm1, int4 const& xmm2) noexcept
	{
		return _mm_cmpgt_epi32(xmm1, xmm2);
	}

	/**
	 * @brief Computes the logical LESS THAN for each elements.
	 * @return int4 ( xmm1.x < xmm2.x ? 0xFFFFFFFF : 0, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static int4 lt(int4 const& xmm1, int4 const& xmm2) noexcept
	{
		return _mm_cmplt_epi32(xmm1, xmm2);
	}

	/**
	 * @brief Computes the logical GREATER THAN OR EQUAL for each elements.
	 * @return int4 ( xmm1.x >= xmm2.x ? 0xFFFFFFFF : 0, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static int4 ge(int4 const& xmm1, int4 const& xmm2) noexcept
	{
		return _mm_xor_si128(_mm_cmplt_epi32(xmm1, xmm2), constants4i::elem_mask); // Not Less-Than
	}

	/**
	 * @brief Computes the logical LESS THAN OR EQUAL for each elements.
	 * @return int4 ( xmm1.x <= xmm2.x ? 0xFFFFFFFF : 0, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static int4 le(int4 const& xmm1, int4 const& xmm2) noexcept
	{
		return _mm_xor_si128(_mm_cmpgt_epi32(xmm1, xmm2), constants4i::elem_mask); // Not Greater-Than
	}

/**
	 * @brief Computes the negation for each elements.
	 * @return int4 ( - xmm2.x, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static int4 neg(int4 const& xmm) noexcept
	{
		return _mm_sub_epi32(_mm_setzero_si128(), xmm);
	}

	/**
	 * @brief Computes the addition for each elements.
	 * @return int4 ( xmm1.x + xmm2.x, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static int4 add(int4 const& xmm1, int4 const& xmm2) noexcept
	{
		return _mm_add_epi32(xmm1, xmm2);
	}

	/**
	 * @brief Computes the subtraction for each elements.
	 * @return int4 ( xmm1.x - xmm2.x, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static int4 sub(int4 const& xmm1, int4 const& xmm2) noexcept
	{
		return _mm_sub_epi32(xmm1, xmm2);
	}

	/**
	 * @brief Computes the multiplication for each elements.
	 * @return int4 ( xmm1.x * xmm2.x, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static int4 mul(int4 const& xmm1, int4 const& xmm2) noexcept
	{
		return _mm_mullo_epi32(xmm1, xmm2);
	}

	/**
	 * @brief Computes the division for each elements.
	 * @return int4 ( xmm1.x / xmm2.x, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static int4 div(int4 const& xmm1, int4 const& xmm2) noexcept
	{
		return _mm_div_epi32(xmm1, xmm2);
	}

	/**
	 * @brief Computes the `min()` for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static int4 min(int4 const& xmm1, int4 const& xmm2) noexcept
	{
		return _mm_min_epi32(xmm2, xmm1);
	}

	/**
	 * @brief Computes the `min()` for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static int4 min(int4 const& xmm1, int4 const& xmm2, int4 const& xmm3) noexcept
	{
		return _mm_min_epi32(_mm_min_epi32(xmm3, xmm2), xmm1);
	}

	/**
	 * @brief Computes the `max()` for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static int4 max(int4 const& xmm1, int4 const& xmm2) noexcept
	{
		return _mm_max_epi32(xmm2, xmm1);
	}

	/**
	 * @brief Computes the `max()` for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static int4 max(int4 const& xmm1, int4 const& xmm2, int4 const& xmm3) noexcept
	{
		return _mm_max_epi32(_mm_max_epi32(xmm3, xmm2), xmm1);
	}

	/**
	 * @brief Computes the `clamp()` for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static int4 clamp(int4 const& low, int4 const& high, int4 const& value) noexcept
	{
		return _mm_max_epi32(_mm_min_epi32(value, high), low);
	}

	/**
	 * @brief Calculates the `abs()` for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static int4 abs(int4 const& xmm) noexcept
	{
		return _mm_and_epi32(constants4i::sign_mask, xmm);
	}

	/**
	 * @brief Calculates the bitwise AND for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static int4 bit_and(int4 const& xmm1, int4 const& xmm2) noexcept
	{
		return _mm_and_epi32(xmm1, xmm2);
	}

	/**
	 * @brief Calculates the bitwise AND-NOT for each elements.
	 * @returns int4( ( ~xmm1.x ) & xmm2.x, same for yzw )
	 */
	[[nodiscard]][[msvc::forceinline]] static int4 bit_addnot(int4 const& xmm1, int4 const& xmm2) noexcept
	{
		return _mm_andnot_epi32(xmm1, xmm2);
	}

	/**
	 * @brief Calculates the bitwise OR for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static int4 bit_or(int4 const& xmm1, int4 const& xmm2) noexcept
	{
		return _mm_or_epi32(xmm1, xmm2);
	}

	/**
	 * @brief Calculates the bitwise XOR for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static int4 bit_xor(int4 const& xmm1, int4 const& xmm2) noexcept
	{
		return _mm_xor_epi32(xmm1, xmm2);
	}

	/**
	 * @brief Does the logical right-shifting for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static int4 bit_rshift(int4 const& xmm, int const& bits) noexcept
	{
		return _mm_srli_epi32(xmm, bits);
	}

	/**
	 * @brief Does the logical left-shifting for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static int4 bit_lshift(int4 const& xmm, int const& bits) noexcept
	{
		return _mm_slli_epi32(xmm, bits);
	}

	/**
	 * @brief Does the arithmetic right-shifting for each elements.
	 */
	[[nodiscard]][[msvc::forceinline]] static int4 bit_ashift(int4 const& xmm, int const& bits) noexcept
	{
		return _mm_srai_epi32(xmm, bits);
	}

	/**
	 * @brief Returns an integer bit-mask (0x00 - 0x0f) based on the sign-bit for each elements.
	 * @return Bit 0 = sign(xmm.x), Bit 1 = sign(xmm.y), Bit 2 = sign(xmm.z), Bit 3 = sign(xmm.w)
	 */
	[[nodiscard]][[msvc::forceinline]] static int4 copy_sign(int4 const& xmm, int4 const& sign) noexcept
	{
		return _mm_or_epi32(_mm_and_epi32(constants4i::sign_bit, sign), _mm_andnot_epi32(constants4i::sign_bit, xmm));
	}
}



namespace se
{
	[[nodiscard]][[msvc::forceinline]] static float4 cast(int4 const& xmm) noexcept
	{
		return _mm_cvtepi32_ps(xmm);
	}

	[[nodiscard]][[msvc::forceinline]] static int4 cast(float4 const& xmm) noexcept
	{
		return _mm_cvttps_epi32(xmm);
	}

	[[nodiscard]][[msvc::forceinline]] static int4 round_to_int4(float4 const& xmm) noexcept
	{
		return _mm_cvtps_epi32(xmm);
	}
}