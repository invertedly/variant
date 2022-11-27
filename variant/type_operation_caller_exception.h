#pragma once

#include <string>
#include <exception>

namespace custom_variant
{
	namespace _internal
	{
		class type_operation_caller_exception final : public std::exception
		{
		public:
			explicit type_operation_caller_exception(const std::string& message) :
				exception(message.data()) { }
		};
	}
}