#pragma once

#include "type_index_constraint.h"
#include "variant_type_constraint.h"

namespace custom_variant
{
	template <
		size_t I,
		variant_type_constraint ... Types
	>
	requires type_index_contraint<I, Types... >
	struct variant_alternative;

	template <
		size_t I,
		variant_type_constraint CurType,
		variant_type_constraint ... NextTypes
	>
	struct variant_alternative<I, CurType, NextTypes ... > final
	{
		using type = typename variant_alternative<I - 1, NextTypes ... >::type;
	};

	template <
		variant_type_constraint CurType,
		variant_type_constraint ... NextTypes
	>
	struct variant_alternative<0, CurType, NextTypes ... > final
	{
		using type = CurType;
	};

	template <
		size_t I,
		variant_type_constraint ... Types
	>
	requires type_index_contraint<I, Types... >
	using variant_alternative_t = typename variant_alternative<I, Types...>::type;
}
