// Copyright (c) 2024 Fong ZiSing. All rights reserved.
//
//     GridAccelerator.hpp
//

#pragma once

#include "Starry/Core/Public/Vector.hpp"

#include <vector>
#include <span>
#include <functional>
#include <algorithm>



namespace se
{
	class grid2d_accelerator
	{
	private:
		struct alignas(16) resource
		{
			vec2 position;
			int32_t next;
			int32_t reserved;
		};

		struct alignas(16) grid
		{
			std::size_t num;
			int32_t head;
			int32_t reserved;
		};
		
		static constexpr int32_t grid_bits = 3;
		static constexpr int32_t grid_limit = 16384;
		int32_t cols, rows;
		std::vector<grid> grids;
		std::vector<resource> resources;


	public:
		grid2d_accelerator(const vec2i& scene_size)
			: cols(std::min(grid_limit, (scene_size.x + (1 << grid_bits) - 1) >> grid_bits))
			, rows(std::min(grid_limit, (scene_size.y + (1 << grid_bits) - 1) >> grid_bits))
			, grids(cols * rows, grid{})
		{}

		/**
		 * @brief rebuild grid's indexing information.
		 * @details 更新索引信息
		 */
		void rebuild(std::span<const vec2> positions) noexcept;

		void query_near_of(std::size_t index, vec2 position, float radius, std::function<void(int, float, vec2 const)>&& callable) const;


	private:
		/** Non-copyable. */
		grid2d_accelerator(const grid2d_accelerator&) = delete;
		grid2d_accelerator& operator = (const grid2d_accelerator&) = delete;

		/** Disable new. */
		void* operator new (std::size_t, void*) = delete;
		void* operator new (std::size_t) = delete;
	};
}