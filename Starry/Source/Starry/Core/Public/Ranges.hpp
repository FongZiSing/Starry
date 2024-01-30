// Copyright (c) 2024 Fong ZiSing. All rights reserved.
//
//     Ranges.hpp
//

#pragma once



namespace se::ranges
{
	template <typename C, typename V, typename S>
	class indexed_iterator
	{
	public:
		using container_type = C;
		using value_type = V;
		using size_type = S;


	private:
		container_type& container;
		size_type iterate_index;


	public:
		constexpr indexed_iterator(container_type& in_container, size_type start_index = 0) noexcept
			: container(in_container)
			, iterate_index(start_index)
		{}

		constexpr bool operator != (indexed_iterator const& rhs) const noexcept
		{
			return iterate_index != rhs.iterate_index;
		}

		constexpr indexed_iterator& operator++ () noexcept
		{
			++iterate_index;
			return *this;
		}

		constexpr indexed_iterator operator++ (int) noexcept
		{
			indexed_iterator iter { *this };
			++iterate_index;
			return iter;
		}

		constexpr indexed_iterator& operator-- () noexcept
		{
			--iterate_index;
			return *this;
		}
		constexpr indexed_iterator operator-- (int) noexcept
		{
			indexed_iterator iter { *this };
			--iterate_index;
			return iter;
		}

		constexpr indexed_iterator& operator+= (size_type const offset) noexcept
		{
			iterate_index += offset;
			return *this;
		}

		constexpr indexed_iterator operator+ (size_type const offset) const noexcept
		{
			indexed_iterator iter { *this };
			return iter += offset;
		}

		constexpr indexed_iterator& operator-= (size_type const offset) noexcept
		{
			return *this += -offset;
		}

		constexpr indexed_iterator operator- (size_type const offset) const noexcept
		{
			indexed_iterator iter { *this };
			return iter -= offset;
		}

		constexpr decltype(auto) operator* () const
		{
			struct indexed_pair { size_type size; value_type& value; };
			return indexed_pair { iterate_index, container[iterate_index] };
		}

		constexpr value_type* operator-> () const
		{
			return &container[iterate_index];
		}

		constexpr explicit operator bool () const
		{
			return container.size() > iterate_index;
		}

		constexpr size_type index() const noexcept
		{
			return iterate_index;
		}
	};



	template <typename C>
	class indexed_view
	{
	public:
		using container_type = C;
		using value_type = container_type::value_type;
		using size_type = container_type::size_type;
		using iterator = indexed_iterator<container_type, value_type, size_type>;


	private:
		container_type& container;


	public:
		constexpr indexed_view(container_type& in_container) noexcept
			: container(in_container)
		{}

		constexpr iterator begin() noexcept
		{
			return iterator { container };
		}

		constexpr iterator end() noexcept
		{
			return iterator { container, container.size() };
		}
	};



	template <typename C>
	class indexed_const_view
	{
	public:
		using container_type = C;
		using value_type = container_type::value_type;
		using size_type = container_type::size_type;
		using const_iterator = indexed_iterator<const container_type, const value_type, size_type>;


	private:
		container_type& container;


	public:
		constexpr indexed_const_view(container_type& in_container) noexcept
			: container(in_container)
		{}

		constexpr const_iterator begin() const noexcept
		{
			return const_iterator{ container };
		}

		constexpr const_iterator end() const noexcept
		{
			return const_iterator{ container, container.size() };
		}
	};
}