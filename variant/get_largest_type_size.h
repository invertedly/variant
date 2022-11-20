#pragma once

#include "variant_type_constraint.h"
#include <algorithm>

namespace custom_variant
{
	template<variant_type_constraint ... Types>
	struct largest_type_size_getter;

	template<variant_type_constraint CurType,
		variant_type_constraint ... Types>
	struct largest_type_size_getter<CurType, Types...> final
	{
		constexpr static size_t get() noexcept
		{
			return std::max(
				sizeof(CurType),
				largest_type_size_getter<Types...>::get()
			);
		}
	};

	template<variant_type_constraint CurType>
	struct largest_type_size_getter<CurType> final
	{
		constexpr static size_t get() noexcept
		{
			return sizeof(CurType);
		}
	};

	template<variant_type_constraint ... Types>
	constexpr size_t get_largest_type_size() noexcept
	{
		return largest_type_size_getter<Types...>::get();
	}
}