#pragma once

#include <cstddef>
#include <functional>

#include "get_type_index.h"
#include "has_type.h"

#include "head_type_getter.h"

#include "destructor.h"
#include "variant_exception.h"

namespace custom_variant
{
	template <typename T>
	concept variant_is_default_constructible_v = variant_type_constraint<T>
		&& std::is_default_constructible_v<T>;

	template <typename T>
	concept variant_is_constructible_from_v = variant_type_constraint<T>
		&& std::is_move_constructible_v<T>;

	template<variant_type_constraint ... Types>
	class variant final
	{
		std::byte memory_[std::max({sizeof(Types)...})];

		size_t type_index_;

		bool valueless_by_exception_ = false;

		using head_type = typename head_type_getter<Types...>::head_type;

	public:

		explicit constexpr variant() noexcept (std::is_nothrow_default_constructible_v<head_type>)
		requires variant_is_default_constructible_v<head_type> :	memory_(),
																	type_index_(get_type_index<
																		head_type, 
																		Types...
																	>())
		{
			new (memory_) head_type();
		}

		explicit constexpr variant(const variant& other)
		requires (!std::is_copy_constructible_v<Types...>) = delete;

		explicit constexpr variant(const variant& other)
		requires std::is_trivially_copy_constructible_v<Types...> = default;

		explicit constexpr variant(const variant& other) :	memory_(),
															type_index_(other.type_index_)
		{
			if (other.valueless_by_exception())
			{
				valueless_by_exception_ = true;
				return;
			}

			new(memory_) 
		}

		template< variant_is_constructible_from_v T >
		explicit constexpr variant(T&& t) noexcept (std::is_nothrow_move_constructible_v<decltype(t)>) : memory_(),
																										 type_index_(get_type_index<
																											 T, 
																											 Types...
																										 >())
		{
			new (memory_) T(std::forward<T>(t));
		}

		constexpr bool valueless_by_exception() const noexcept
		{
			return valueless_by_exception_;
		}

		~variant()
		{
			destruct<Types...>(memory_, type_index_);
		}

		[[nodiscard]] constexpr std::size_t index() const noexcept
		{
			return type_index_;
		}
	};
}