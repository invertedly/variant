#pragma once

#include "type_alternative_list.h"

namespace custom_variant
{
	template<variant_type_constraint CheckType,
		variant_type_constraint CurType,
		variant_type_constraint ... NextTypes>
	struct type_checker final
	{
		static bool check(const type_alternative_list<CurType, NextTypes ... >& t) noexcept
		{
			if (std::is_same_v<CheckType, CurType>)
			{
				return true;
			}
			return type_checker<CheckType, NextTypes...>::check(t.next_types);
		}
	};

	template<variant_type_constraint CheckType,
		variant_type_constraint CurType>
	struct type_checker<CheckType, CurType> final
	{
		static bool check(const type_alternative_list<CurType>& t) noexcept
		{
			return std::is_same_v<CheckType, CurType>;
		}
	};

	template<variant_type_constraint CheckType,
		variant_type_constraint ... Types>
	bool has_type(const type_alternative_list<Types...>& t) noexcept
	{
		return type_checker<CheckType, Types...>::check(t);
	}

	template<typename CheckType,
		variant_type_constraint ... Types>
	requires (!variant_type_constraint<CheckType>)
	bool has_type(const type_alternative_list<Types...>& t) noexcept
	{
		return false;
	}
}