#pragma once

#include "type_operation_caller.h"

//namespace custom_variant
//{
//	template<
//		typename Head,
//		typename ... Tail
//	>
//	void destruct_type(void* memory, const size_t type_index)
//	{
//		auto operation = []<typename T>(void* memory)
//		{
//			static_cast<T*>(memory)->~T();
//		};
//
//		type_operation_caller<0, Head, Tail...>::call_operation(memory, type_index, operation);
//	}
//}