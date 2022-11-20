#pragma once

namespace custom_variant
{
	template<size_t I, typename ... Types>
	concept type_index_contraint = (I < sizeof...(Types));
}