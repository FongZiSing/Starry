#pragma once

#include <tuple>
#include <string_view>
#include <type_traits>



namespace se::ecs
{
	/**
	 * @brief Compile-time string.
	 * @details 编译期字符串
	 */
	template<typename _char_t, std::size_t n>
	struct string_literal
	{
		_char_t str[n]{ '\0' };


	public:
		consteval string_literal() = default;

		consteval string_literal(const _char_t(&in_str)[n])
		{
			for (std::size_t i = 0; i < n; ++i)
			{
				str[i] = in_str[i];
			}
		}


	public:
		template <std::size_t m>
		consteval bool equals(string_literal<_char_t, m> other) const
		{
			if (n != m)
			{
				return false;
			}
			for (std::size_t i = 0; i < n; ++i)
			{
				if (str[i] != other.str[i])
				{
					return false;
				}
			}
			return true;
		}
	};
}



namespace se::ecs::detail
{
	/**
	 * @brief Extract the member object pointer.
	 * @details 解析成员变量指针，获取成员变量的类型、其所在类的类型
	 */
	template <typename>
	struct member_trait
	{
		using member_type = void;
		using class_type = void;
	};

	template <typename _1t, typename _2t>
	struct member_trait<_1t _2t::*>
	{
		using member_type = _1t;
		using class_type = _2t;
	};



	/**
	 * @brief Get name string and hash value of type in compile-time.
	 * @details 编译期将类型转换成字符串、以及生成 hash 值
	 */
	template <typename _t>
	struct typename_trait
	{
		consteval static std::string_view name()
		{
			constexpr std::string_view function_ctname{ __FUNCTION__ };
			constexpr std::string_view function_suffix{ ">::name" };
			constexpr std::size_t npos = std::size_t(-1);

			// Struct cases.
			constexpr std::string_view search_prefix1{ "<struct " };
			constexpr std::size_t search_offset1 = function_ctname.find(search_prefix1);
			if constexpr (search_offset1 != npos)
			{
				constexpr std::size_t offset = search_offset1 + search_prefix1.size();
				constexpr std::size_t length = function_ctname.size() - function_suffix.size() - offset;
				return { function_ctname.data() + offset, length };
			}

			// Class cases.
			constexpr std::string_view search_prefix2{ "<class " };
			constexpr std::size_t search_offset2 = function_ctname.find(search_prefix2);
			if constexpr (search_offset2 != npos)
			{
				constexpr std::size_t offset = search_offset2 + search_prefix2.size();
				constexpr std::size_t length = function_ctname.size() - function_suffix.size() - offset;
				return { function_ctname.data() + offset, length };
			}

			// Universal cases.
			constexpr std::string_view search_prefix3{ "typename_trait<" };
			constexpr std::size_t search_offset3 = function_ctname.find(search_prefix3);
			if constexpr (search_offset3 != npos)
			{
				constexpr std::size_t offset = search_offset3 + search_prefix3.size();
				constexpr std::size_t length = function_ctname.size() - function_suffix.size() - offset;
				return { function_ctname.data() + offset, length };
			}

			return {};
		}

		consteval static uint32_t value()
		{
			constexpr std::string_view type_name = name();
			uint32_t hash = 0;
			for (char c : type_name)
			{
				hash = 31 * hash + (uint32_t)c;
			}
			return hash;
		}
	};
}



namespace se::ecs
{
	/**
	 * @brief Extract the member object pointer.
	 * @details 解析成员变量指针，获取成员变量的类型、其所在类的类型
	 */
	template <typename _t, typename _u = std::remove_cv_t<_t>> requires std::is_member_object_pointer_v<_t>
	using type_of_member_t = detail::member_trait<_u>::member_type;

	template <typename _t, typename _u = std::remove_cv_t<_t>> requires std::is_member_object_pointer_v<_t>
	using class_of_member_t = detail::member_trait<_u>::class_type;

	template <typename _t>
	concept member_pointer = std::is_member_object_pointer_v<_t>;



	/**
	 * @brief Get name string of type in compile-time.
	 * @details 编译期将类型转换成字符串
	 */
	template<typename _t>
	consteval auto name_of_type()
	{
		constexpr std::string_view name = detail::typename_trait<_t>::name();
		constexpr std::size_t length = name.size();
		string_literal<char, length + 1> result{};
		name.copy(result.str.data(), length);
		return result;
	}



	/**
	 * @brief Get hash value of type in compile-time.
	 * @details 编译期生成类型的 hash 值
	 */
	template <typename _t>
	consteval uint32_t hashcode_of_type()
	{
		return detail::typename_trait<_t>::value();
	}
}



namespace se::ecs
{
	/**
	 * @brief Stores compile-time reflection information for a member object.
	 * @details 成员变量的编译期反射信息
	 */
	template <string_literal name, member_pointer auto pointer, std::size_t index>
	struct meta_field
	{
		using name_type = decltype(name);
		using pointer_type = decltype(pointer);
		using member_type = type_of_member_t<pointer_type>;
		using class_type = class_of_member_t<pointer_type>;


	public:
		static constexpr std::size_t static_bytes = sizeof(member_type);
		static constexpr name_type static_name = name;
		static constexpr pointer_type static_pointer = pointer;
		static constexpr std::size_t static_index = index;
	};



	/**
	 * @brief Stores compile-time reflection information of an object,
	 *        in addition to itself, as well as the information of its member objects.
	 * @details 类及其成员变量的编译期反射信息
	 */
	template <typename _object_t, typename _fields_warpper_t>
	struct meta_class
	{
		template <std::size_t nth>
		using nth_field_t = typename std::tuple_element_t<nth, _fields_warpper_t>;


	public:
		static constexpr std::size_t npos = std::size_t(-1);
		static constexpr std::size_t size = std::tuple_size_v<_fields_warpper_t>;
		const _fields_warpper_t fields;


	public:
		consteval std::size_t num_fields() const { return size; }

		template <string_literal... name>
		constexpr auto get_fields() const
		{
			return std::make_tuple(get_field<name>()...);
		}

		template <string_literal name>
		constexpr auto get_field() const
		{
			return get_field_impl<name>(std::make_index_sequence<size>{});
		}

		template <typename _callable_t>
		constexpr void for_each_fields(_callable_t&& callable) const
		{
			for_each_fields_impl(callable, std::make_index_sequence<size>{});
		}

		template <typename _setter_t>
		constexpr void copy_fields(_object_t const& src, _setter_t&& dst) const
		{
			copy_fields_impl(src, dst, std::make_index_sequence<size>{});
		}

		template <typename _getter_t, typename _callable_t, typename... _args_t>
		constexpr void visit_fields(_getter_t&& src, _callable_t&& callable, _args_t&&... args) const
		{
			visit_fields_impl(src, callable, std::make_index_sequence<size>{}, args...);
		}

		template <string_literal... name>
		constexpr auto filter_object() const
		{
			auto subfields = std::make_tuple(get_field<name>()...);
			return meta_class<_object_t, decltype(subfields)>{ subfields };
		}


	private:
		template <string_literal name, std::size_t... nth>
		constexpr auto get_field_impl(std::index_sequence<nth...>) const
		{
			auto subfields = std::tuple_cat(
				std::conditional_t<
				nth_field_t<nth>::static_name.equals(name),
				std::tuple<nth_field_t<nth>>,
				std::tuple<>
				>{}...
			);

			static_assert(std::tuple_size_v<decltype(subfields)>, "[Starry Engine] Unknown attribute name, please check your input!");
			return std::get<0>(subfields);
		}

		template <typename _callable_t, std::size_t... nth>
		constexpr void for_each_fields_impl(_callable_t&& callable, std::index_sequence<nth...>) const
		{
			(
				callable(std::get<nth>(fields)),
				...
			);
		}

		template <typename _setter_t, std::size_t... nth>
		constexpr void copy_fields_impl(_object_t const& src, _setter_t&& dst, std::index_sequence<nth...>) const
		{
			(
				(*(typename nth_field_t<nth>::member_type*)(dst(nth_field_t<nth>::static_index)) = src.*(nth_field_t<nth>::static_pointer)),
				...
			);
		}

		template <typename _getter_t, typename _callable_t, std::size_t... nth, typename... _args_t>
		constexpr void visit_fields_impl(_getter_t&& src, _callable_t&& callable, std::index_sequence<nth...>, _args_t&&... args) const
		{
			callable(
				args...,
				*(typename nth_field_t<nth>::member_type*)(src(nth_field_t<nth>::static_index))...
			);
		}
	};
}



namespace se::ecs
{
	template <typename _object_t>
	struct reflection
	{
		consteval static auto config()
		{
			constexpr auto fields = _object_t::config();
			constexpr auto object = meta_class<_object_t, decltype(fields)>{ fields };
			return object;
		}
	};



	template <string_literal name, member_pointer auto pointer>
	struct attribute
	{
		static constexpr auto static_name = name;
		static constexpr auto static_pointer = pointer;
	};

	template <typename>
	struct is_attribute : public std::false_type {};

	template <auto... args>
	struct is_attribute<attribute<args...>> : public std::true_type {};



	template <typename _field_tuples_t, std::size_t... nth>
	consteval static auto register_meta_field_unchecked(_field_tuples_t&&, std::index_sequence<nth...>&&)
	{
		return std::make_tuple(
			ecs::meta_field<
			std::tuple_element_t<nth, _field_tuples_t>::static_name,
			std::tuple_element_t<nth, _field_tuples_t>::static_pointer,
			nth>
		{}...
		);
	}

	template <typename... _attrs_t>
	consteval static auto register_meta_field(_attrs_t&&... attrs)
	{
		if constexpr (!std::conjunction_v<is_attribute<_attrs_t>...>)
		{
			static_assert(std::conjunction_v<is_attribute<_attrs_t>...>, "[Starry Engine] Has invalid attribute, are attributes all use \'se::attribute_t\'?");
		}
		else
		{
			return register_meta_field_unchecked(
				std::make_tuple(attrs...),
				std::index_sequence_for<_attrs_t...>{}
			);
		}
	}
}