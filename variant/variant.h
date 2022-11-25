#pragma once

#include <cstddef>

#include "destruct_type.h"
#include "variant_alternative.h"
#include "get_type_index.h"
#include "bad_variant_access_exception.h"
#include "has_type.h"

namespace custom_variant
{
	template <typename T>
	concept is_default_constructible_v = std::is_default_constructible_v<T>;

	template<variant_type_constraint ... Types>
	class variant final
	{
		std::byte memory_[std::max({sizeof(Types)...})];
		size_t type_index_;

	public:

		template <is_default_constructible_v T0 = variant_alternative_t<0, Types...>>
		constexpr variant() noexcept (std::is_nothrow_default_constructible_v<T0>) : memory_{}, type_index_{}
		{
			new(memory_) T0();
		}

		template <variant_type_constraint T>
		requires has_type_v<T, Types...>
		explicit constexpr variant(T&& t) noexcept (std::is_nothrow_constructible_v<T, T>) : memory_{}, type_index_{ get_type_index<T, Types...>() }
		{
			new(memory_) T(std::forward<T>(t));
		}

		constexpr variant(const variant& other) requires (!(std::is_copy_constructible_v<Types> && ...)) = delete;

		constexpr variant(const variant& other) : memory_{}, type_index_{other.type_index_}
		{
			copy_construct<Types...>(const_cast<std::byte*>(memory_), type_index_, const_cast<std::byte*>(other.memory_));
		}

		constexpr variant(variant&& other) requires (!(std::is_move_constructible_v<Types> && ...)) = delete;

		constexpr variant(variant&& other) noexcept ((std::is_nothrow_move_constructible_v<Types> && ...)) : memory_{}, type_index_(other.type_index_)
		{
			move
		}

		template <size_t I>
		requires type_index_contraint<I, Types...>
		[[nodiscard]] constexpr decltype(auto) get() const
		{
			if (I != type_index_)
			{
				throw bad_variant_access("wrong type index");
			}
			return *reinterpret_cast<variant_alternative_t<I, Types...>*>(const_cast<std::byte*>(memory_));
		}

		~variant()
		{
			destruct_type<Types...>(memory_, type_index_);
		}

		[[nodiscard]] constexpr std::size_t index() const noexcept
		{
			return type_index_;
		}
	};
}