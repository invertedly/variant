#pragma once

#include "variant_alternative.h"

namespace custom_variant
{
	template< std::size_t I, typename ... Types >
	constexpr variant_alternative_t<I, variant<Types ...>>& 
		get(const variant<Types...>& v)
	{
		if (v.index() == I)
		{
			return *reinterpret_cast<variant_alternative_t<I, Types...>>(memory_);
		}

		throw bad_variant_access("index is out of range");
	}
}