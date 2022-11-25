#include "pch.h"

#include "../variant/variant.h"

#include <variant>

TEST(variant_default_ctor, default)
{
	{
		custom_variant::variant<const int, std::string> v{};
		decltype(auto) val = v.get<0>();
		EXPECT_EQ(val, int{});
		EXPECT_EQ(v.index(), 0);
	}
	{
		custom_variant::variant<int, std::string> v{};
		decltype(auto) val = v.get<0>();
		EXPECT_EQ(val, int{});
		EXPECT_EQ(v.index(), 0);
	}
	{
		custom_variant::variant<const std::string, int> v{};
		decltype(auto) val = v.get<0>();
		EXPECT_EQ(val, std::string{});
		EXPECT_EQ(v.index(), 0);
	}
	{
		custom_variant::variant<std::string> v{};
		decltype(auto) val = v.get<0>();
		EXPECT_EQ(val, std::string{});
		EXPECT_EQ(v.index(), 0);
	}
}

TEST(variant_default_ctor, has_not_default_constructible_type)
{
	class not_default_constructible final
	{
	public:
		not_default_constructible() = delete;
	};

	custom_variant::variant<int, not_default_constructible> v{};
	decltype(auto) val = v.get<0>();
	EXPECT_EQ(val, int{});
	EXPECT_EQ(v.index(), 0);
}

TEST(variant_copy_ctor, trivially_copy_constructible)
{
	{
		custom_variant::variant<int> v_old{};
		custom_variant::variant<int> v_new(v_old);
	}

	{
		custom_variant::variant<int, double> v_old(1.1);
		custom_variant::variant<int, double> v_new(v_old);
	}
}

TEST(variant_copy_ctor, not_trivially_copy_constructible)
{
	class not_trivially_copyable
	{
	public:
		not_trivially_copyable() = default;
		not_trivially_copyable(const not_trivially_copyable&) {}
	};

	constexpr bool not_trivially_copy_constructible = !std::is_trivially_copy_constructible_v<not_trivially_copyable>;
	EXPECT_TRUE(not_trivially_copy_constructible);

	constexpr bool default_constructible = std::is_default_constructible_v<not_trivially_copyable>;
	EXPECT_TRUE(default_constructible);

	{
		custom_variant::variant<not_trivially_copyable, std::string> v_old{};
		custom_variant::variant<not_trivially_copyable, std::string> v_new(v_old);
	}

	{
		custom_variant::variant<not_trivially_copyable, std::string> v_old{std::string{}};
		custom_variant::variant<not_trivially_copyable, std::string> v_new(v_old);
	}
}

TEST(variant_convert_ctor, default)
{
	{
		custom_variant::variant<int, float> var(1);
		EXPECT_EQ(var.index(), 0);
		EXPECT_EQ(var.get<0>(), 1);
	}

	{
		custom_variant::variant<int, double> var(1.1);
		EXPECT_EQ(var.index(), 1);
		EXPECT_EQ(var.get<1>(), 1.1);
	}

	//{
	//	int a = 1;
	//	custom_variant::variant<int, double> var(a);
	//	EXPECT_EQ(var.index(), 1);
	//	EXPECT_EQ(var.get<1>(), 1.1);
	//}
}

TEST(has_type_v, default)
{
	{
		constexpr bool b = custom_variant::has_type_v<int, float, std::string, int>;
		EXPECT_TRUE(b);
	}

	{
		constexpr bool b = custom_variant::has_type_v<int, int, std::string, float>;
		EXPECT_TRUE(b);
	}

	{
		constexpr bool b = custom_variant::has_type_v<const int, float, std::string, int>;
		EXPECT_FALSE(b);
	}

	{
		constexpr bool b = custom_variant::has_type_v<const int, float, const int, int>;
		EXPECT_TRUE(b);
	}
}

TEST(variant_alternative, single_type)
{
	constexpr bool cv_int_t = std::is_same_v<custom_variant::variant_alternative_t<0, const volatile int>, const volatile int>;
	EXPECT_TRUE(cv_int_t);

	class A {};

	constexpr bool cv_user_class_t = std::is_same_v<custom_variant::variant_alternative_t<0, const volatile A>, const volatile A>;
	EXPECT_TRUE(cv_user_class_t);
}

TEST(variant_alternative, several_types)
{
	constexpr bool cv_int_first = std::is_same_v<custom_variant::variant_alternative_t<0, const volatile int, int, int>, const volatile int>;
	EXPECT_TRUE(cv_int_first);

	constexpr bool cv_int_mid = std::is_same_v<custom_variant::variant_alternative_t<1, int, const volatile int, int>, const volatile int>;
	EXPECT_TRUE(cv_int_mid);

	constexpr bool cv_int_end = std::is_same_v<custom_variant::variant_alternative_t<2, int, int, const volatile int>, const volatile int>;
	EXPECT_TRUE(cv_int_end);
}

TEST(variant_alternative, several_types_repetitive)
{
	constexpr bool int_first = std::is_same_v<custom_variant::variant_alternative_t<0, int, int, int>, int>;
	EXPECT_TRUE(int_first);

	constexpr bool int_mid = std::is_same_v<custom_variant::variant_alternative_t<1, int, int, int>, int>;
	EXPECT_TRUE(int_mid);

	constexpr bool int_last = std::is_same_v<custom_variant::variant_alternative_t<2, int, int, int>, int>;
	EXPECT_TRUE(int_last);
}

TEST(variant_get, valid_index)
{
	{
		custom_variant::variant<int, float> var(1);
		EXPECT_EQ(var.index(), 0);
		EXPECT_EQ(var.get<0>(), 1);
	}

	{
		custom_variant::variant<int, double> var(1.1);
		EXPECT_EQ(var.index(), 1);
		EXPECT_EQ(var.get<1>(), 1.1);
	}
}

TEST(variant_get, invalid_index)
{
	{
		custom_variant::variant<int, float> var(1);
		EXPECT_EQ(var.index(), 0);
		EXPECT_THROW(var.get<1>(), custom_variant::bad_variant_access);
	}

	{
		custom_variant::variant<int, double> var(1.1);
		EXPECT_EQ(var.index(), 1);
		EXPECT_THROW(var.get<0>(), custom_variant::bad_variant_access);
	}
}

TEST(get_type_index, valid_type)
{
	using check_type = int;

	{
		constexpr size_t index = custom_variant::get_type_index<check_type, int, float, char>();
		EXPECT_EQ(index, 0);
	}

	{
		constexpr size_t index = custom_variant::get_type_index<check_type, float, int, char>();
		EXPECT_EQ(index, 1);
	}

	{
		constexpr size_t index = custom_variant::get_type_index<check_type, float, char, int>();
		EXPECT_EQ(index, 2);
	}

	{
		constexpr size_t index = custom_variant::get_type_index<check_type, int>();
		EXPECT_EQ(index, 0);
	}
}

TEST(get_type_index, invalid_type)
{
	using check_type = const int;

	{
		constexpr size_t index = custom_variant::get_type_index<check_type, int, float, char>();
		EXPECT_EQ(index, custom_variant::variant_npos);
	}

	{
		constexpr size_t index = custom_variant::get_type_index<check_type, int>();
		EXPECT_EQ(index, custom_variant::variant_npos);
	}
}
