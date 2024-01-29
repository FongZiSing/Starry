// Copyright (c) 2024 Fong ZiSing. All rights reserved.
//
//     Scene.hpp
//

#pragma once

#include "Starry/Core/Public/Ranges.hpp"
#include "Particle.hpp"

#include <chrono>
#include <type_traits>



namespace se
{
	enum class visitor_flag : unsigned
	{
		position_only,
		velocity_only,
		basic_particle, // position and velocity.
		particle,
	};

	template<visitor_flag flag>
	using selected_visitor_t = std::conditional_t<
		flag == visitor_flag::position_only, particle_t::position_t, std::conditional_t<
		flag == visitor_flag::velocity_only, particle_t::velocity_t, std::conditional_t<
		flag == visitor_flag::basic_particle, basic_particle_intermediate_t, std::conditional_t<
		flag == visitor_flag::particle, particle_intermediate_t, void
		>>>>;
}



namespace se
{
	class scene2d_t
	{
	private:
		particle_system_t system;
		vec2i_t size;
	

	protected:
		scene2d_t(vec2i_t const& in_scene_size, vec2i_t const& in_field_size, unsigned in_detection_radius) noexcept
		{
			size = in_scene_size;
		}


	public:
		friend scene2d_t make_scene(vec2i_t const& in_scene_size, vec2i_t const& in_field_size, unsigned in_detection_radius);
	
		const vec2i_t& scene_size() const noexcept { return size; }

		template<typename G>
		void spawn_particle(G&& generator)
		{
			std::vector<particle_t> particles = generator(size);
			system.reset(particles.size());

			for (auto&& [index, particle] : ranges::indexed_view(particles))
			{
				system.init_at(index, particle);
			}
		}

		template<visitor_flag flag, typename V>
		long long tick(V&& visitor)
		{
			auto start = std::chrono::high_resolution_clock::now();

			system.for_each<selected_visitor_t<flag>>(std::move(visitor));
			
			auto end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
			return duration.count();
		}

		template<visitor_flag flag, typename V>
		void render(V&& visitor) const
		{
			system.for_each_const<selected_visitor_t<flag>>(std::move(visitor));
		}


	private:
		/** Non-copyable. */
		scene2d_t(const scene2d_t&) = delete;
		scene2d_t& operator = (const scene2d_t&) = delete;

		/** Disable new. */
		void* operator new (std::size_t, void*) = delete;
		void* operator new (std::size_t) = delete;
	};



	scene2d_t make_scene(vec2i_t const& in_scene_size, vec2i_t const& in_field_size = { 10, 10 }, unsigned in_detection_radius = 10)
	{
		return scene2d_t(in_scene_size, in_field_size, in_detection_radius);
	}
}