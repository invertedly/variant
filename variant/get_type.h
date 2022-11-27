#pragma once

#include "type_index_constraint.h"
#include "variant_type_constraint.h"

namespace custom_variant
{
	namespace _internal
	{
		template <
			size_t I,
			variant_type_constraint ... Types
		>
			requires type_index_contraint<I, Types... >
		struct get_type;

		template <
			size_t I,
			variant_type_constraint CurType,
			variant_type_constraint ... NextTypes
		>
		struct get_type<I, CurType, NextTypes ... > final
		{
			using type = typename get_type<I - 1, NextTypes ... >::type;
		};

		template <
			variant_type_constraint CurType,
			variant_type_constraint ... NextTypes
		>
		struct get_type<0, CurType, NextTypes ... > final
		{
			using type = CurType;
		};

		template <
			size_t I,
			variant_type_constraint ... Types
		>
			requires type_index_contraint<I, Types... >
		using get_type_t = typename get_type<I, Types...>::type;
	}
}
