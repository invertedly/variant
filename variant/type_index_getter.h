#pragma once

#include "type_alternative_list.h"

namespace custom_variant
{

	template<variant_type_constraint CheckType,
		variant_type_constraint CurType,
		variant_type_constraint ... NextTypes>
	struct type_index_getter final
	{
		static size_t get(
			const type_alternative_list<CurType, Nex)
	};
}