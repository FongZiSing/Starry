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


namespace se::attribute_list { static constexpr se::ecs::string_literal position = "position"; }
#define se_register_particle_attributes   se::ecs::register_meta_field
#define se_make_attribute(name, pointer)  se::ecs::attribute<name, pointer>{}
#define se_make_position(pointer)         se::ecs::attribute<se::attribute_list::position, pointer>{}



namespace se
{
	class particle_system : public ecs::system
	{
	private:
		ecs::entity_manager entity_mgr; // TODO
		ecs::component_manager component_mgr;


	public:
		particle_system() = default;

		template <typename _user_particle_t>
		std::size_t num() const noexcept
		{
			return entity_mgr.num<_user_particle_t>();
		}

		template <typename _user_particle_t>
		void generate_particle(auto&& particles)
		{
			entity_mgr.generate<_user_particle_t>(particles);
			component_mgr.generate_entity_components<_user_particle_t>(std::move(particles));
		}

		template<typename _user_particle_t, ecs::string_literal attr>
		[[msvc::forceinline]] auto any_of() const
		{
			return component_mgr.any_of<_user_particle_t, attr>();
		}
		
		template<typename _user_particle_t, ecs::string_literal... attrs>
		[[msvc::forceinline]] void for_each(auto&& callable)
		{
			component_mgr.for_each<_user_particle_t, attrs...>(std::move(callable));
		}

		template<typename _user_particle_t, ecs::string_literal attrs>
		[[msvc::forceinline]] void for_each(auto&& callable) const
		{
			component_mgr.for_each<_user_particle_t, attrs>(std::move(callable));
		}
	};
}