#include "pch.h"

#include "../variant/variant.h"

TEST(alt_type, several_types) {
	class A {};

	custom_variant::type_alternative_list<int, char, std::string, A, std::vector<int>> alt_t;

	EXPECT_EQ(custom_variant::has_type<int>(alt_t), true);
	EXPECT_EQ(custom_variant::has_type<char>(alt_t), true);
	EXPECT_EQ(custom_variant::has_type<std::string>(alt_t), true);
	EXPECT_EQ(custom_variant::has_type<A>(alt_t), true);
	EXPECT_EQ(custom_variant::has_type<std::vector<int>>(alt_t), true);

	EXPECT_EQ(custom_variant::has_type<bool>(alt_t), false);
	EXPECT_EQ(custom_variant::has_type<std::vector<char>>(alt_t), false);
}

TEST(alt_type, const_type) {
	custom_variant::type_alternative_list<const int, std::vector<const int>, const std::vector<int>> alt_const_t;

	EXPECT_EQ(custom_variant::has_type<const int>(alt_const_t), true);
	EXPECT_EQ(custom_variant::has_type<std::vector<const int>>(alt_const_t), true);
	EXPECT_EQ(custom_variant::has_type<const std::vector<int>>(alt_const_t), true);	
	
	EXPECT_EQ(custom_variant::has_type<int>(alt_const_t), false);
	EXPECT_EQ(custom_variant::has_type<std::vector<int>>(alt_const_t), false);
	EXPECT_EQ(custom_variant::has_type<const std::vector<const int>>(alt_const_t), false);

	custom_variant::type_alternative_list<int> alt_t;

	EXPECT_EQ(custom_variant::has_type<int>(alt_t), true);
	EXPECT_EQ(custom_variant::has_type<const int>(alt_t), false);
}

TEST(alt_type, pointer) {
	custom_variant::type_alternative_list<int*> alt_t;

	EXPECT_EQ(custom_variant::has_type<int*>(alt_t), true);
}

