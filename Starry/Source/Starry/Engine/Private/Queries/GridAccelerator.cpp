// Copyright (c) 2024 Fong ZiSing. All rights reserved.
//
//     GridAccelerator.cpp
//

#include "Starry/Engine/Public/Queries/GridAccelerator.hpp"

#define STARRY_USE_INTRINSIC
#ifdef STARRY_USE_INTRINSIC
#include "Starry/Core/Private/Intrinsic.hpp"
#endif


namespace se
{
	void grid2d_accelerator::rebuild(std::span<const se::vec2> positions) noexcept
	{
		resources.resize(positions.size());
		std::memset(grids.data(), 0, grids.size() * sizeof(grid));

		const se::vec2* pos_ptr = positions.data();
		resource* res_ptr = resources.data();
		std::size_t size = positions.size() / 4;

		const auto insert_lambda = [this, res_ptr](int32_t slot, resource* last, const vec2* position)
			{
				grid& curr = grids[slot];
				int32_t prev = curr.head;
				last->position = *position;
				last->next = prev;
				curr.head = (int32_t)std::distance(res_ptr, last) + 1;
				curr.num++;
			};


#ifndef STARRY_USE_INTRINSIC
		for (std::size_t i = 0; i < size; ++i)
		{
			for (std::size_t j = 0; j < 4; ++j)
			{
				vec2 const& position = *pos_ptr;
				int32_t integer_x = int32_t(position.x) >> grid_bits;
				int32_t integer_y = int32_t(position.y) >> grid_bits;
				int32_t clamped_x = std::clamp(integer_x, 0, cols - 1);
				int32_t clamped_y = std::clamp(integer_y, 0, rows - 1);
				int32_t slot = clamped_x + (clamped_y * cols);

				[[msvc::forceinline_calls]] insert_lambda(slot, res_ptr++, pos_ptr++);
			}
		}
#else
		const int4 min_0000 = zero4i();
		const int4 max_xxxx = make(cols - 1);
		const int4 max_yyyy = make(rows - 1);
		const int4 stride = make(cols);

		for (std::size_t i = 0; i < size; ++i)
		{
			float4 floating_pos1 = load((const float*)pos_ptr);
			float4 floating_pos2 = load((const float*)(pos_ptr + 2));
			float4 floating_xxxx = shuffle<0, 2, 0, 2>(floating_pos1, floating_pos2);
			float4 floating_yyyy = shuffle<1, 3, 1, 3>(floating_pos1, floating_pos2);

			int4 shifted_xxxx = bit_ashift(cast(floating_xxxx), grid_bits);
			int4 shifted_yyyy = bit_ashift(cast(floating_yyyy), grid_bits);
			int4 clamped_xxxx = clamp(min_0000, max_xxxx, shifted_xxxx);
			int4 clamped_yyyy = clamp(min_0000, max_yyyy, shifted_yyyy);
			int4 correct_slot = add(mul(stride, clamped_yyyy), clamped_xxxx);

			[[msvc::forceinline_calls]]
			{
				insert_lambda(extract<0>(correct_slot), res_ptr++, pos_ptr++);
				insert_lambda(extract<1>(correct_slot), res_ptr++, pos_ptr++);
				insert_lambda(extract<2>(correct_slot), res_ptr++, pos_ptr++);
				insert_lambda(extract<3>(correct_slot), res_ptr++, pos_ptr++);
			}
		}
#endif
		while (pos_ptr != positions.data() + positions.size())
		{
			vec2 const& position = *pos_ptr;
			int32_t integer_x = int32_t(position.x) >> grid_bits;
			int32_t integer_y = int32_t(position.y) >> grid_bits;
			int32_t clamped_x = std::clamp(integer_x, 0, cols - 1);
			int32_t clamped_y = std::clamp(integer_y, 0, rows - 1);
			int32_t slot = clamped_x + (clamped_y * cols);

			[[msvc::forceinline_calls]]
			insert_lambda(slot, res_ptr++, pos_ptr++);
		}
	}



	void grid2d_accelerator::query_near_of(std::size_t index, vec2 position, float radius, std::function<void(int, float, vec2 const)>&& callable) const
	{
		if (radius <= 0) [[unlikely]]
		{
			return;
		}

		// TODO

		const float radius_squared = math::square(radius);
		const int32_t integer_radius = (int32_t)radius;
		const int32_t grid_radius = (integer_radius + (1 << grid_bits) - 1) >> grid_bits;

		int32_t slot_x = int32_t(position.x) >> grid_bits;
		int32_t slot_y = int32_t(position.y) >> grid_bits;

		int4 box = make(-grid_radius, -grid_radius, grid_radius, grid_radius);
		box = add(box, make(slot_x, slot_y, slot_x, slot_y));
		box = clamp(zero4i(), make(cols, rows, cols, rows), box);
		
		for (int32_t i = box.m128i_i32[0]; i < box.m128i_i32[2]; ++i)
		{
			for (int32_t j = box.m128i_i32[1]; j < box.m128i_i32[3]; ++j)
			{
				int32_t slot = i + (j * cols);
				const grid& grid = grids[slot];
				int32_t head = grid.head;
				
				while (head)
				{
					resource const& res = resources[head - 1];
					if (index != head - 1) // ignore self.
					{
						vec2 const& found_position = res.position;
						if (float distance_squared = position.distance_squared(found_position); distance_squared < radius_squared)
						{
							callable(head, distance_squared, found_position);
						}
					}
					head = res.next;
				}
			}
		}
	}
}