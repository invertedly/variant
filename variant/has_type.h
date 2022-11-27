#pragma once

#include <type_traits>

namespace custom_variant
{
	namespace _internal
	{
		template <typename T, typename ... Types>
		constexpr bool has_type_v = std::disjunction_v<std::is_same<T, Types>...>;
	}
}
