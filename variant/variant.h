#pragma once

#include "type_checker.h"

namespace custom_variant
{
	template<variant_type_constraint ... Types>
	class variant final
	{
		void* object_ptr_;
	public:
		// in ctor we get max size type and allocate enough memory
		// also we save size of memory allocated?

		constexpr variant() noexcept
		{
		}

		template< class T >
		constexpr variant(T&& t) noexcept
		{
			//if has type t, initialize ptr, otherwise ...
		}

	};
}