#pragma once

#include <string>
#include <exception>

namespace _internal_custom_variant
{
	class type_operation_caller_exception final : public std::exception
	{
	public:
		explicit type_operation_caller_exception(const std::string& message) :
			exception(message.data()) { }
	};
}