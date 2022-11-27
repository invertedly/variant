#pragma once

#include "variant.h"

#include "type_index_constraint.h"

namespace custom_variant
{
	template<size_t I, typename T>
	struct variant_alternative;

	template<
		size_t I,
		variant_type_constraint CurType, 
		variant_type_constraint ... NextTypes
	>
requires type_index_contraint<I, CurType, NextTypes ... >
	struct variant_alternative<I, variant<CurType, NextTypes ... > > final
	{
		using type = typename variant_alternative<I - 1, variant<NextTypes ...> >::type;
	};

	template<
		variant_type_constraint CurType,
		variant_type_constraint ... NextTypes
	>
	requires type_index_contraint<0, CurType, NextTypes...>
	struct variant_alternative<0, variant<CurType, NextTypes ... > > final
	{
		using type = CurType;
	};

	template <size_t I, typename T>
	using variant_alternative_t = typename variant_alternative<I, T>::type;

	template <std::size_t I, typename T>
	struct variant_alternative<I, const T> final
	{
		using type = std::add_const_t<variant_alternative_t<I, T>>;
	};

	template <std::size_t I, typename T>
	struct variant_alternative<I, volatile T> final
	{
		using type = std::add_volatile_t<variant_alternative_t<I, T>>;
	};

	template <std::size_t I, typename T>
	struct variant_alternative<I, const volatile T> final
	{
		using type = std::add_cv_t<variant_alternative_t<I, T>>;
	};
}