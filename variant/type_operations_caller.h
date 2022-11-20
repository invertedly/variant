#pragma once

#include "destructor_exception.h"

namespace custom_variant
{
	template<size_t I, typename ... Types>
	struct type_operations_caller;

	template<size_t I, typename Head, typename ... Tail>
	struct type_operations_caller<I, Head, Tail...> final
	{


		constexpr static void destruct(void* memory, const size_t index)
		{
			if (I == index)
			{
				static_cast<Head*>(memory)->~Head();
				return;
			}

			type_operations_caller<I + 1, Tail...>::destruct(memory, index);
		}
	};

	template<size_t I>
	struct type_operations_caller<I> final
	{
		constexpr static void destruct(void* memory, const size_t index)
		{
			throw destructor_exception("type index is out of range");
		}
	};

	template<typename ... Types>
	constexpr void destruct(void* memory, const size_t index)
	{
		type_operations_caller<0, Types...>::destruct(memory, index);
	}
}