#pragma once

#include <type_traits>

namespace custom_variant
{
	template<typename T>
	concept variant_type_constraint = std::is_object_v<T> && !std::is_array_v<T> && std::is_destructible_v<T>;
}