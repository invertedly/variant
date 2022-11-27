#pragma once

#include "variant_type_constraint.h"

namespace custom_variant
{
	inline constexpr std::size_t variant_npos = -1;

	template<
		size_t Index,
		variant_type_constraint CheckType,
		variant_type_constraint ... Types
	>
	struct type_index_getter;

	template<
		size_t Index,
		variant_type_constraint CheckType,
		variant_type_constraint CurType,
		variant_type_constraint ... Types
	>
	struct type_index_getter<Index, CheckType, CurType, Types...> final
	{
		constexpr static size_t get() noexcept
		{
			return std::is_same_v<CheckType, CurType> ? Index : type_index_getter<Index + 1, CheckType, Types...>::get();
		}
	};

	template<
		size_t Index,
		variant_type_constraint CheckType,
		variant_type_constraint CurType
	>
	struct type_index_getter<Index, CheckType, CurType> final
	{
		constexpr static size_t get() noexcept
		{
			return std::is_same_v<CheckType, CurType> ? Index : variant_npos;
		}
	};

	template<
		variant_type_constraint CheckType,
		variant_type_constraint ... Types
	>
	constexpr size_t get_type_index() noexcept
	{
		return custom_variant::type_index_getter<0, CheckType, Types ... >::get();
	}
}