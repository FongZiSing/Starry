// Copyright (c) 2024 Fong ZiSing. All rights reserved.
//
//     Component.hpp
//

#pragma once

#include <vector>



namespace se::ecs
{
	template <typename T>
	struct component_t
	{
	public:
		using attribute_t = T;
		using attribute_array_t = std::vector<attribute_t>;
		using size_type = attribute_array_t::size_type;
		using value_type = attribute_array_t::value_type;


	private:
		attribute_array_t attributes;


	public:
		[[msvc::forceinline]] void resize(size_type num)
		{
			attributes.resize(num);
		}

		[[nodiscard]][[msvc::forceinline]] size_type size() const noexcept
		{
			return attributes.size();
		}

		[[nodiscard]][[msvc::forceinline]] attribute_t& at(size_type const index) noexcept
		{
			// TODO: add a check.
			return attributes.data()[index];
		}

		[[nodiscard]][[msvc::forceinline]] attribute_t const& at(size_type const index) const noexcept
		{
			// TODO: add a check.
			return attributes.data()[index];
		}

		template<typename V>
		void for_each(V&& visitor)
		{
			for (attribute_t& attr : attributes)
			{
				visitor(attr);
			}
		}

		template<typename V>
		void for_each(V&& visitor) const
		{
			for (attribute_t const& attr : attributes)
			{
				visitor(attr);
			}
		}
	};
}
