#pragma once

#include <string>
#include <exception>

namespace custom_variant
{
	class destructor_exception final : public std::exception
	{
	public:
		explicit destructor_exception(const std::string& message) :
			exception(message.data()) { }
	};
}