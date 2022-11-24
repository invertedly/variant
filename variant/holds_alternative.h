#pragma once

#include "variant_type_constraint.h"
#include "variant.h"

namespace custom_variant
{
	template< 
		variant_type_constraint T,
		variant_type_constraint... Types
	>
	requires (has_type<T, Types...>())
	constexpr bool holds_alternative(const variant<Types...>& v) noexcept
	{
		return v.index() == get_type_index<T, Types...>();
	}
}
