#pragma once

#include <string>
#include <exception>

namespace custom_variant
{
	class out_of_list_exception final : public std::exception
	{
	public:
		explicit out_of_list_exception(const std::string& message) : 
			exception(message.data()) { }
	};
}