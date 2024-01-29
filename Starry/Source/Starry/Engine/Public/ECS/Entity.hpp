// Copyright (c) 2024 Fong ZiSing. All rights reserved.
//
//     Entity.hpp
//

#pragma once

#include <cstdint>
#include <vector>



namespace se::ecs
{
	struct entity_t
	{
	public:
		using id_t = int32_t;
		using id_array_t = std::vector<id_t>;
		using size_type = id_array_t::size_type;
		using value_type = id_array_t::value_type;


	private:
		id_array_t ids;


	public:
		constexpr void resize(size_type num)
		{
			ids.resize(num);
		}

		[[nodiscard]] constexpr size_type size() const noexcept
		{
			return ids.size();
		}

		[[nodiscard]] constexpr id_t& at(size_type const index) noexcept
		{
			// TODO: add a check.
			return ids.data()[index];
		}

		[[nodiscard]] constexpr id_t const& at(size_type const index) const noexcept
		{
			// TODO: add a check.
			return ids.data()[index];
		}


	public:
		template<typename V>
		void for_each(V&& visitor)
		{
			for (id_t& id : ids)
			{
				visitor(id);
			}
		}

		template<typename V>
		void for_each(V&& visitor) const
		{
			for (id_t const& id : ids)
			{
				visitor(id);
			}
		}
	};
}
