#pragma once

#include <string>

namespace custom_variant
{
	class bad_variant_access final : public std::exception
	{
	public:
		explicit bad_variant_access(const std::string& message) : std::exception(message.data()) {}
	};
}