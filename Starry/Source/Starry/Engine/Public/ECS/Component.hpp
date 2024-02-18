// Copyright (c) 2024 Fong ZiSing. All rights reserved.
//
//     Component.hpp
//

#pragma once

#include "Reflection.hpp"

#include <vector>
#include <unordered_map>
#include <span>



namespace se::ecs
{
	struct component
	{
	private:
		const std::size_t bytes;
		std::size_t num;
		std::vector<uint8_t> buffer;


	public:
		explicit component(std::size_t in_bytes) noexcept
			: bytes(in_bytes)
			, num(0)
		{}

		[[msvc::forceinline]] void resize(std::size_t const in_size)
		{
			num = in_size;
			buffer.resize(in_size * bytes);
		}

		[[nodiscard]][[msvc::forceinline]] std::size_t size() const noexcept
		{
			return num;
		}

		[[nodiscard]][[msvc::forceinline]] uint8_t* data(std::size_t const start = 0) noexcept
		{
			return buffer.data() + start * bytes;
		}

		[[nodiscard]][[msvc::forceinline]] uint8_t const* data(std::size_t const start = 0) const noexcept
		{
			return buffer.data() + start * bytes;
		}

		template<typename _cast_t = uint8_t>
		void for_each(auto& callable)
		{
			std::size_t fixed_size = bytes * num / sizeof(_cast_t);
			_cast_t* first = reinterpret_cast<_cast_t*>(buffer.data());
			const _cast_t* last = first + fixed_size;

			while (first != last)
			{
				callable(*first);
				++first;
			}
		}

		template<typename _cast_t = uint8_t>
		void for_each(auto&& callable) const
		{
			std::size_t fixed_size = bytes * num / sizeof(_cast_t);
			const _cast_t* first = reinterpret_cast<const _cast_t*>(buffer.data());
			const _cast_t* last = first + fixed_size;

			while (first != last)
			{
				callable(*first);
				++first;
			}
		}
	};
}



namespace se::ecs
{
	/**
	 * @brief Components archetype.
	 * @details 组件原型
	 */
	class component_archetypes
	{
	public:
		using value_type = std::vector<component>;
		using hash_type = uint32_t;
		using hash_map = std::unordered_map<hash_type, value_type>;


	private:
		hash_map map{};


	public:
		template <typename _object_t>
		[[msvc::forceinline]] value_type& add_unique()
		{
			constexpr uint32_t hash = hashcode_of_type<_object_t>();
			std::pair<hash_map::iterator, bool> result = map.try_emplace(hash);
			value_type& components = result.first->second;

			if (result.second)
			{
				constexpr auto reflect = reflection<_object_t>::config();
				components.reserve(reflect.num_fields());
				reflect.for_each_fields(
					[&components](auto& field)
					{
						components.emplace_back(field.static_bytes);
					});

			}

			return components;
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
	/**
	 * @brief Components manager.
	 * @details 组件管理器
	 */
	class component_manager
	{
	private:
		component_archetypes archetypes;


	public:
		/**
		 * @brief Allocate all components of a entity.
		 * @details 创建并填充一个实体的所有组件
		 */
		template <typename _object_t>
		void generate_entity_components(std::vector<_object_t>&& objects)
		{
			// Gets compile-time reflection information of entity.
			constexpr auto reflect = reflection<_object_t>::config();

			// Allocation.
			std::vector<component>& components = archetypes.add_unique<_object_t>();
			for (component& per_component : components)
			{
				per_component.resize(per_component.size() + objects.size());
			}

			for (std::size_t i = 0; i < objects.size(); ++i)
			{
				reflect.copy_fields(
					objects[i],
					[i, &components](std::size_t component_index)
					{
						return components[component_index].data(i);
					}
				);
			}
		}

		/**
		 * @brief Retrieves the given component of the given entity.
		 * @return 返回给定实体的特定组件
		 */
		template<typename _object_t, string_literal attr>
		decltype(auto) any_of() const
		{
			// Gets compile-time reflection information of entity.
			constexpr auto reflect = reflection<_object_t>::config();

			// Calculates the hash value of entity in compile-time, uses it to find the entity.
			constexpr uint32_t hash = hashcode_of_type<_object_t>();
			const std::vector<component>& components = archetypes[hash];

			// Indexes to the corresponding component based on reflection information and return it.
			constexpr auto field = reflect.get_field<attr>();
			using data_t = decltype(field)::member_type;
			return std::span<const data_t>((const data_t*)components[field.static_index].data(), components[field.static_index].size());
		}

		/**
		 * @brief Traverses the given component of the given entity.
		 * @details 遍历给定实体的特定组件
		 */
		template<typename _object_t, string_literal... attrs>
		void for_each(auto&& callable)
		{
			// Gets compile-time reflection information of entity.
			constexpr auto reflect = reflection<_object_t>::config();

			// Calculates the hash value of entity in compile-time, uses it to find the entity.
			constexpr uint32_t hash = hashcode_of_type<_object_t>();
			std::vector<component>& components = archetypes[hash];

			// Indexes to the corresponding component based on reflection information and traverse it.
			if constexpr (sizeof...(attrs) == 0)
			{
				const std::size_t component_size = components[0].size();
				for (std::size_t i = 0; i < component_size; ++i)
				{
					reflect.visit_fields(
						[i, &components](std::size_t component_index)
						{
							return components[component_index].data(i);
						},
						callable, i
					);
				}
			}
			else if constexpr (sizeof...(attrs) == 1)
			{
				constexpr auto field = reflect.get_field<attrs...>();
				using data_t = decltype(field)::member_type;
				components[field.static_index].for_each<data_t>(callable);
			}
			else
			{
				constexpr auto filter_reflect = reflect.filter_object<attrs...>();
				const std::size_t component_size = components[0].size();
				for (std::size_t i = 0; i < component_size; ++i)
				{
					filter_reflect.visit_fields(
						[i, &components](std::size_t component_index)
						{
							return components[component_index].data(i);
						},
						callable, i
					);
				}
			}
		}

		/**
		 * @brief Traverses the given component of the given entity.
		 * @details 遍历给定实体的特定组件
		 */
		template<typename _object_t, string_literal... attrs>
		void for_each(auto&& callable) const
		{
			// Gets compile-time reflection information of entity.
			constexpr auto reflect = reflection<_object_t>::config();
			constexpr uint32_t hash = hashcode_of_type<_object_t>();

			// Indexes to the corresponding component based on reflection information and traverse it.
			const std::vector<component>& components = archetypes[hash];
			if constexpr (sizeof...(attrs) == 0)
			{
				const std::size_t component_size = components[0].size();
				for (std::size_t i = 0; i < component_size; ++i)
				{
					reflect.visit_fields(
						[i, &components](std::size_t component_index)
						{
							return components[component_index].data(i);
						},
						callable
					);
				}
			}
			else if constexpr (sizeof...(attrs) == 1)
			{
				constexpr auto field = reflect.get_field<attrs...>();
				using data_t = decltype(field)::member_type;
				components[field.static_index].for_each<data_t>(callable);
			}
			else
			{
				constexpr auto filter_reflect = reflect.filter_object<attrs...>();
				const std::size_t component_size = components[0].size();
				for (std::size_t i = 0; i < component_size; ++i)
				{
					filter_reflect.visit_fields(
						[i, &components](std::size_t component_index)
						{
							return components[component_index].data(i);
						},
						callable
					);
				}
			}
		}
	};
}
