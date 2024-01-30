// Copyright (c) 2024 Fong ZiSing. All rights reserved.
//
//     Entity.hpp
//

#pragma once

#include "Reflection.hpp"

#include <cstdint>
#include <vector>
#include <span>
#include <unordered_map>



namespace se::ecs
{
	struct entity
	{
		int32_t id;
	};
}



namespace se::ecs
{
	class entity_archetypes
	{
	public:
		using value_type = std::vector<entity>;
		using hash_type = uint32_t;
		using hash_map = std::unordered_map<hash_type, value_type>;
	
	
	private:
		hash_map map{};


	public:
		[[msvc::forceinline]] value_type& add_unique(hash_type hash)
		{
			return map[hash];
		}

		[[nodiscard]][[msvc::forceinline]] value_type& operator[] (hash_type hash)
		{
		    return map.at(hash);
		}

		[[nodiscard]][[msvc::forceinline]] value_type const& operator[] (hash_type hash) const
		{
			return map.at(hash);
		}
	};
}



namespace se::ecs
{
	class entity_manager
	{
	private:
		entity_archetypes archetypes;


	public:
		/**
		 * @brief Retrieves the count of the given entity type.
		 * @details 返回实体
		 */
		template <typename _object_t>
		std::size_t num() const
		{
			// Gets compile-time reflection information of entity.
			constexpr auto reflect = reflection<_object_t>::config();
			constexpr auto hash = hashcode_of_type<_object_t>();

			return archetypes[hash].size();
		}

		/**
		 * @brief Allocate entities.
		 * @details 分配实体
		 */
		template <typename _object_t>
		void generate(std::span<const _object_t> objects)
		{
			// Gets compile-time reflection information of entity.
			constexpr auto reflect = reflection<_object_t>::config();
			constexpr uint32_t hash = hashcode_of_type<_object_t>();
			
			// Allocation.
			std::vector<entity>& entities = archetypes.add_unique(hash);
			std::size_t count = objects.size();
			std::size_t index = entities.size();
			entities.reserve(index + count);
			while (count--)
			{
				entities.emplace_back(entity_archetypes::hash_type(index++));
			}
		}
	};
}
