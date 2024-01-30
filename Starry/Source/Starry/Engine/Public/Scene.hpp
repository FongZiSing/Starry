// Copyright (c) 2024 Fong ZiSing. All rights reserved.
//
//     Scene.hpp
//

#pragma once

#include "Starry/Core/Public/Ranges.hpp"
#include "Particle.hpp"
#include "Accelerator.hpp"

#include <type_traits>



namespace se
{
	class scene2d
	{
	private:
		particle_system system;
		vec2i size;
		grid2d_accelerator accel;
	

	protected:
		scene2d(vec2i const& in_scene_size) noexcept
			: size{ in_scene_size }
			, accel{ in_scene_size }
		{}


	public:
		friend scene2d make_scene(vec2i const& in_scene_size);
	
		const vec2i& scene_size() const noexcept
		{
			return size;
		}

		const grid2d_accelerator* get_accelerator() const noexcept
		{
			return &accel;
		}

		template<typename _user_particle_t, typename _generator_t>
		void generate_particle(_generator_t&& generator)
		{
			system.generate_particle<_user_particle_t>(generator(size));
		}

		/**
		 * @brief Retrieves the given attribute of all particle instances.
		 * @details 返回粒子属性
		 */
		template<typename _user_particle_t, ecs::string_literal attr>
		[[msvc::forceinline]] auto query_any_of() const
		{
			return system.any_of<_user_particle_t, attr>();
		}

		/**
		 * @brief Invokes a callable to the attribute(s) of all particle instances, used to query.
		 * @details 查询粒子属性
		 */
		template<typename _user_particle_t, ecs::string_literal... attrs>
		[[msvc::forceinline]] void query_for_each(auto&& callable) const
		{
			system.for_each<_user_particle_t, attrs...>(std::move(callable));
		}

		/**
		 * @brief Invokes a callable to the attribute(s) of all particle instances, used to update.
		 * @details 更新粒子属性
		 */
		template<typename _user_particle_t, ecs::string_literal... attrs>
		[[msvc::forceinline]] void update_for_each(auto&& callable)
		{
			system.for_each<_user_particle_t, attrs...>(std::move(callable));
		}

		/**
		 * @brief Calls before update all particles.
		 * @details 准备更新粒子属性
		 */
		template<typename _user_particle_t>
		[[msvc::forceinline]] void begin_update()
		{
			// TOFIX
			accel.rebuild(query_any_of<_user_particle_t, attribute_list::position>());
		}

		/**
		 * @brief Calls if update phase is finished.
		 * @details 粒子属性更新完成
		 */
		template<typename _user_particle_t>
		[[msvc::forceinline]] void end_update()
		{
			// TODO
		}


	private:
		/** Non-copyable. */
		scene2d(const scene2d&) = delete;
		scene2d& operator = (const scene2d&) = delete;

		/** Disable new. */
		void* operator new (std::size_t, void*) = delete;
		void* operator new (std::size_t) = delete;
	};



	scene2d make_scene(vec2i const& in_scene_size)
	{
		return scene2d(in_scene_size);
	}
}