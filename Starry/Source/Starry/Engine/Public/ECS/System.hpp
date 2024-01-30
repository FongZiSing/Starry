// Copyright (c) 2024 Fong ZiSing. All rights reserved.
//
//     System.hpp
//

#pragma once

#include <cstddef>



namespace se::ecs
{
	class system
	{
	public:
		system() = default;


	private:
		/** Non-copyable. */
		system(system const&) = delete;
		system& operator = (system const&) = delete;

		/** Disable new. */
		void* operator new (std::size_t, void*) = delete;
		void* operator new (std::size_t) = delete;
	};
}