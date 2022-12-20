#pragma once

#include <algorithm>

#include "type_operation_caller.h"
#include "get_type.h"
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

		template <is_default_constructible_v T0 = _internal::get_type_t<0, Types...>>
		explicit constexpr variant() noexcept (std::is_nothrow_default_constructible_v<T0>) : memory_{}, type_index_{}
		{
			new(memory_) T0();
		}

		template <typename T, typename RemoveReferenceT = std::remove_reference_t<T>>
		requires (
			_internal::has_type_v<RemoveReferenceT, Types...> && 
			!std::is_same_v<RemoveReferenceT, variant> && 
			variant_type_constraint<RemoveReferenceT>
		)
		explicit constexpr variant(T&& t) noexcept (std::is_nothrow_constructible_v<RemoveReferenceT, decltype(std::forward<T>(t))>)
			: memory_{},
			  type_index_{ _internal::get_type_index<RemoveReferenceT, Types...>() }
		{
			new(memory_) RemoveReferenceT(std::forward<T>(t));
		}

		constexpr variant(const variant& other) requires (!(std::is_copy_constructible_v<Types> && ...)) = delete;

		constexpr variant(const variant& other) : memory_{}, type_index_{other.type_index_}
		{
			_internal::copy_construct<Types...>(const_cast<std::byte*>(memory_), type_index_, const_cast<std::byte*>(other.memory_));
		}

		constexpr variant(variant&& other) requires (!(std::is_move_constructible_v<Types> && ...)) = delete;

		constexpr variant(variant&& other) noexcept ((std::is_nothrow_move_constructible_v<Types> && ...)) : memory_{}, type_index_(other.type_index_)
		{
			swap(other);
		}

		constexpr variant& operator=(const variant& rhs)
		{
			if (this != &rhs)
			{
				type_index_ = rhs.type_index_;
				_internal::copy_construct<Types...>(const_cast<std::byte*>(memory_), type_index_, const_cast<std::byte*>(rhs.memory_));
			}
			return *this;
		}

		constexpr variant& operator=(variant&& rhs) noexcept(((std::is_nothrow_move_constructible_v<Types> && std::is_nothrow_move_assignable_v<Types>) && ...))
		{
			if (this != &rhs)
			{
				type_index_ = rhs.type_index_;
				swap(rhs);
			}
			return *this;
		}

		constexpr void swap(variant& rhs) noexcept(std::is_nothrow_swappable_v<decltype(memory_)>)
		{
			if (this == &rhs)
			{
				return;
			}
			std::swap(memory_, rhs.memory_);
		}

		template <typename T>
		constexpr bool holds_alternative() noexcept
		{
			return type_index_ == _internal::get_type_index<T, Types...>();
		}

		template <size_t I>
		requires type_index_contraint<I, Types...>
		[[nodiscard]] constexpr decltype(auto) get() const
		{
			if (I != type_index_)
			{
				throw bad_variant_access("wrong type index");
			}
			return *reinterpret_cast<_internal::get_type_t<I, Types...>*>(const_cast<std::byte*>(memory_));
		}

		~variant()
		{
			_internal::destruct_type<Types...>(memory_, type_index_);
		}

		[[nodiscard]] constexpr std::size_t index() const noexcept
		{
			return type_index_;
		}
	};
}