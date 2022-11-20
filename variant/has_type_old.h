#pragma once

#include "type_index_getter.h"

namespace custom_variant
{
	template<variant_type_constraint CheckType,
		variant_type_constraint ... Types>
	bool has_type(const type_alternative_list<Types...>& type_list) noexcept
	{
		bool has_type = true;
		try 
		{
			size_t index = custom_variant::get_type_index<CheckType, Types...>(type_list);
		}
		catch (out_of_list_exception&)
		{
			has_type = false;
		}
		return has_type;
	}

	template<typename CheckType,
		variant_type_constraint ... Types>
		requires (!variant_type_constraint<CheckType>)
	bool has_type(const type_alternative_list<Types...>& type_list) noexcept
	{
		return false;
	}
}