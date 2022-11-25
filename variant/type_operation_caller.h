#pragma once

#include "type_operation_caller_exception.h"

namespace custom_variant
{
	template<size_t I, typename ... Types>
	struct type_operation_caller;

	template<size_t I, typename Head, typename ... Tail>
	struct type_operation_caller<I, Head, Tail...> final
	{
		static void copy_construct(void* memory, const size_t index, void* other_memory)
		{
			if (I == index)
			{
				new (memory) Head(*static_cast<Head*>(other_memory));
				return;
			}

			return type_operation_caller<I + 1, Tail...>::copy_construct(memory, index, other_memory);
		}

		static void move_construct(void* memory, const size_t index, void* other_memory)
		{
			if (I == index)
			{
				new (memory) Head(*static_cast<Head*>(other_memory));
				return;
			}

			return type_operation_caller<I + 1, Tail...>::copy_construct(memory, index, other_memory);
		}

		static void destruct(void* memory, const size_t index)
		{
			if (I == index)
			{
				static_cast<Head*>(memory)->~Head();
				return;
			}

			type_operation_caller<I + 1, Tail...>::destruct(memory, index);
		}
	};

	template<size_t I>
	struct type_operation_caller<I> final
	{
		static void copy_construct(void* memory, const size_t index, void* other_memory)
		{
			throw type_operation_caller_exception("type index is out of range");
		}

		static void destruct(void* memory, const size_t index)
		{
			throw type_operation_caller_exception("type index is out of range");
		}
	};

	template<typename ... Types>
	constexpr void destruct_type(void* memory, const size_t index)
	{
		type_operation_caller<0, Types...>::destruct(memory, index);
	}

	template<typename ... Types>
	void copy_construct(void* memory, const size_t index, void* other_memory)
	{
		type_operation_caller<0, Types...>::copy_construct(memory, index, other_memory);
	}
}