#pragma once

#include "variant_type_constraint.h"
#include "out_of_list_exception.h"

namespace custom_variant
{
	template<size_t Index,
		variant_type_constraint CheckType,
		variant_type_constraint ... Types>
	struct type_index_getter;

	template<size_t Index,
		variant_type_constraint CheckType,
		variant_type_constraint CurType,
		variant_type_constraint ... Types>
	struct type_index_getter<Index, CheckType, CurType, Types...> final
	{
		constexpr static size_t get()
		{
			if (std::is_same_v<CheckType, CurType>)
			{
				return Index;
			}
			return type_index_getter<Index + 1, CheckType, Types...>::get();
		}
	};

	template<size_t Index,
		variant_type_constraint CheckType,
		variant_type_constraint CurType>
	struct type_index_getter<Index, CheckType, CurType> final
	{
		constexpr static size_t get()
		{
			if (std::is_same_v<CheckType, CurType>)
			{
				return Index;
			}
			throw out_of_list_exception("type is out of list");
		}
	};

	template<variant_type_constraint CheckType,
		variant_type_constraint ... Types>
	constexpr size_t get_type_index()
	{
		return custom_variant::type_index_getter<0, CheckType, Types ... >::get();
	}
}