#pragma once
#include <type_traits>

namespace custom_variant
{
	template<typename T>
	concept variant_type_constraint = !std::is_void_v<T> && !std::is_array_v<T> && !std::is_reference_v<T>;

	template<variant_type_constraint ... Types>
	struct type_alternative_list;

	template<variant_type_constraint CurType, 
		variant_type_constraint ... NextTypes>
	struct type_alternative_list<CurType, NextTypes ... > final
	{
		using cur_type = CurType;
		type_alternative_list<NextTypes ... > next_types;
	};

	template<variant_type_constraint CurType>
	struct type_alternative_list<CurType> final
	{
		using cur_type = CurType;
	};
}