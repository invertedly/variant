#pragma once

#include "variant_type_constraint.h"

namespace custom_variant
{
	template <variant_type_constraint SearchType,
		variant_type_constraint ... Types>
	struct type_searcher;

	template <variant_type_constraint SearchType,
		variant_type_constraint CurType,
		variant_type_constraint ... Types>
	struct type_searcher<SearchType, CurType, Types...> final
	{
		constexpr static bool search() noexcept
		{
			if (std::is_same_v<SearchType, CurType>)
			{
				return true;
			}
			return type_searcher<SearchType, Types...>::search();
		}
	};

	template <variant_type_constraint SearchType,
		variant_type_constraint CurType>
	struct type_searcher<SearchType, CurType> final
	{
		constexpr static bool search() noexcept
		{
			return std::is_same_v<SearchType, CurType>;
		}
	};

	template <variant_type_constraint SearchType,
		variant_type_constraint ... Types>
	constexpr bool has_type() noexcept
	{
		return type_searcher<SearchType, Types...>::search();
	}
}
