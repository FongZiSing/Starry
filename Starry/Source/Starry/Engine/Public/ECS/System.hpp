// Copyright (c) 2024 Fong ZiSing. All rights reserved.
//
//     System.hpp
//

#pragma once

#include <cstddef>



namespace se::ecs
{
	class system_t
	{
	public:
		system_t() = default;


	private:
		/** Non-copyable. */
		system_t(system_t const&) = delete;
		system_t& operator = (system_t const&) = delete;

		/** Disable new. */
		void* operator new (std::size_t, void*) = delete;
		void* operator new (std::size_t) = delete;
	};
}