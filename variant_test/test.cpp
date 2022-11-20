#include "pch.h"

#include "../variant/variant.h"

#include "../variant/variant_alternative.h"

#include <type_traits>

TEST(ctor, default)
{
	//custom_variant::variant<int, std::string> v1{};
	//custom_variant::variant<std::string, int> v2;
	custom_variant::variant<std::string> v3;
}

TEST(ctor, from_object)
{
	// copy version?
	int a = 5;
	std::string s = "string";
	//custom_variant::variant<int, std::string> v1(a);
	custom_variant::variant<int, std::string> v2(std::move(s));
}

TEST(destructor, destructor)
{
	{
		constexpr size_t size = sizeof(std::string);
		std::byte memory[size];
		new (memory) std::string("abc");
		custom_variant::destruct<int, float, std::string, std::vector<int>>(memory, 2);
	}

	// valid tests but GOOGLE TEST FUNCTIONS report a syntax error

	//{
	//	constexpr size_t size = sizeof(std::vector<int>);
	//	std::byte memory[size];
	//	new (memory) std::vector<int>();
	//	EXPECT_ANY_THROW(custom_variant::destruct<int, float, std::string, std::vector<int>>(memory, 2));
	//}

	//{
	//	constexpr size_t size = sizeof(std::string);
	//	std::byte memory[size];
	//	new (memory) std::string("abc");
	//	EXPECT_THROW(custom_variant::destruct<int, float, std::string, std::vector<int>>(memory, 4));
	//}
}

TEST(variant_alternative, not_cv_variant)
{
	using my_variant = custom_variant::variant<const int, float, std::string>;

	{
		constexpr bool a = std::is_same_v<const int, custom_variant::variant_alternative_t<0, my_variant>>;
		EXPECT_TRUE(a);
	}

	{
		constexpr bool b = std::is_same_v<float, custom_variant::variant_alternative_t<1, my_variant>>;
		EXPECT_TRUE(b);
	}

	{
		constexpr bool c = std::is_same_v<std::string, custom_variant::variant_alternative_t<2, my_variant>>;
		EXPECT_TRUE(c);
	}
}

TEST(variant_alternative, cv_variant)
{
	using my_variant = custom_variant::variant<int, float, std::string>;

	{
		constexpr bool const_varinant = std::is_same_v<
			const int,
			custom_variant::variant_alternative_t<0, const my_variant>
		>;
		EXPECT_TRUE(const_varinant);
	}

	{
		constexpr bool volatile_variant = std::is_same_v<
			volatile float,
			custom_variant::variant_alternative_t<1, volatile my_variant>
		>;
		EXPECT_TRUE(volatile_variant);
	}

	{
		constexpr bool cv_variant = std::is_same_v<
			const volatile std::string,
			custom_variant::variant_alternative_t<2, const volatile my_variant>
		>;
		EXPECT_TRUE(cv_variant);
	}
}


