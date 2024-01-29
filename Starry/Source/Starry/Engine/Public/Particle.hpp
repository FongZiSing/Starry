// Copyright (c) 2024 Fong ZiSing. All rights reserved.
//
//     Particle.hpp
//

#pragma once

#include "Starry/Core/Public/Vector.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Component.hpp"
#include "ECS/System.hpp"

#include <type_traits>
#include <stdexcept>



namespace se
{
	struct particle_t
	{
		using position_t = vec2_t;
		using velocity_t = vec2_t;
		using custom_attribute_t = vec4_t;


	public:
		position_t position;
		velocity_t velocity;
		custom_attribute_t custom_attributes;
	};

	struct particle_intermediate_t
	{
		particle_t::position_t position;
		particle_t::velocity_t velocity;
		particle_t::custom_attribute_t custom_attributes;
	};

	struct basic_particle_intermediate_t
	{
		particle_t::position_t position;
		particle_t::velocity_t velocity;
	};
}



namespace se
{
	class particle_system_t : public ecs::system_t
	{
	private:
		ecs::entity_t entity;
		ecs::component_t<particle_t::position_t> position_component;
		ecs::component_t<particle_t::velocity_t> velocity_component;
		ecs::component_t<particle_t::custom_attribute_t> custom_attributes_component;


	public:
		particle_system_t() = default;

		std::size_t num() const noexcept
		{
			return entity.size();
		}

		[[msvc::forceinline]] void reset(std::size_t const num)
		{
			entity.resize(num);
			position_component.resize(num);
			velocity_component.resize(num);
			custom_attributes_component.resize(num);
		}

		[[msvc::forceinline]] void init_at(std::size_t const index, particle_t const& particle)
		{
			entity.at(index) = (ecs::entity_t::id_t)index;
			position_component.at(index) = particle.position;
			velocity_component.at(index) = particle.velocity;
			custom_attributes_component.at(index) = particle.custom_attributes;
		}


	public:
		template <typename T, typename V>
		[[msvc::forceinline]] void for_each(V&& visitor)
		{
			if constexpr (std::is_same_v<T, ecs::entity_t::id_t>)
			{
				entity.for_each(std::move(visitor));
			}
			else if constexpr (std::is_same_v<T, particle_t::position_t>)
			{
				position_component.for_each(std::move(visitor));
			}
			else if constexpr (std::is_same_v<T, particle_t::velocity_t>)
			{
				velocity_component.for_each(std::move(visitor));
			}
			else if constexpr (std::is_same_v<T, particle_t::custom_attribute_t>)
			{
				custom_attributes_component.for_each(std::move(visitor));
			}
			else if constexpr (std::is_same_v<T, particle_intermediate_t>)
			{
				for (std::size_t i = 0; i < entity.size(); ++i)
				{
					particle_intermediate_t ref{ position_component.at(i), velocity_component.at(i), custom_attributes_component.at(i) };
					visitor(ref);
				}
			}
			else if constexpr (std::is_same_v<T, basic_particle_intermediate_t>)
			{
				for (std::size_t i = 0; i < entity.size(); ++i)
				{
					basic_particle_intermediate_t ref{ position_component.at(i), velocity_component.at(i) };
					visitor(ref);
				}
			}
			else
			{
				throw std::invalid_argument("[psl] unkown particle attribute!");
			}
		}

		template <typename T, typename V>
		[[msvc::forceinline]] void for_each_const(V&& visitor) const
		{
			if constexpr (std::is_same_v<T, ecs::entity_t::id_t>)
			{
				entity.for_each(std::move(visitor));
			}
			else if constexpr (std::is_same_v<T, particle_t::position_t>)
			{
				position_component.for_each(std::move(visitor));
			}
			else if constexpr (std::is_same_v<T, particle_t::velocity_t>)
			{
				velocity_component.for_each(std::move(visitor));
			}
			else if constexpr (std::is_same_v<T, particle_t::custom_attribute_t>)
			{
				custom_attributes_component.for_each(std::move(visitor));
			}
			else if constexpr (std::is_same_v<T, particle_intermediate_t>)
			{
				for (std::size_t i = 0; i < entity.size(); ++i)
				{
					particle_intermediate_t ref { position_component.at(i), velocity_component.at(i), custom_attributes_component.at(i) };
					visitor(ref);
				}
			}
			else if constexpr (std::is_same_v<T, basic_particle_intermediate_t>)
			{
				for (std::size_t i = 0; i < entity.size(); ++i)
				{
					basic_particle_intermediate_t ref { position_component.at(i), velocity_component.at(i) };
					visitor(ref);
				}
			}
			else
			{
				throw std::invalid_argument("[psl] unkown particle attribute!");
			}
		}
	};
}