#pragma once

#include "variant_type_constraint.h"

namespace custom_variant
{
	template <variant_type_constraint HeadType,
		variant_type_constraint ... OtherTypes>
	struct head_type_getter final
	{
		using head_type = HeadType;
	};
}
