/* Copyright (c) 2016-2018 Jakob Meng, <jakobmeng@web.de>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */


#define BOOST_TEST_MODULE operators_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/fn/plus.hpp>
#include <hbrs/mpl/fn/minus.hpp>
#include <hbrs/mpl/fn/multiply.hpp>
#include <hbrs/mpl/fn/divide.hpp>
#include <hbrs/mpl/fn/modulo.hpp>
#include <hbrs/mpl/fn/equal.hpp>
#include <hbrs/mpl/fn/not_equal.hpp>
#include <hbrs/mpl/fn/less.hpp>
#include <hbrs/mpl/fn/greater.hpp>
#include <hbrs/mpl/fn/less_equal.hpp>
#include <hbrs/mpl/fn/greater_equal.hpp>
#include <hbrs/mpl/fn/not.hpp>
#include <hbrs/mpl/fn/and.hpp>
#include <hbrs/mpl/fn/or.hpp>
#include <hbrs/mpl/fn/preincrement.hpp>
#include <hbrs/mpl/fn/postincrement.hpp>
#include <hbrs/mpl/fn/predecrement.hpp>
#include <hbrs/mpl/fn/postdecrement.hpp>

#include <boost/hana/pair.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/equal.hpp>
#include <type_traits>
#include <array>


BOOST_AUTO_TEST_SUITE(operators_test)
namespace hana = boost::hana;

template<std::size_t Integral>
constexpr std::integral_constant<std::size_t, Integral> ic_c{};

BOOST_AUTO_TEST_CASE(operators_test_1) {
	using namespace hbrs::mpl;
	using namespace hana::literals;
	
	int a = 1;
	int b = 2;
	constexpr int c = 3;
	constexpr int d = 4;
	
	BOOST_CHECK(detail::equal_impl_std_op{}(a, 1));
	BOOST_CHECK((*equal)(a, 1));
	BOOST_CHECK(!(*equal)(a, 2));
	
	static_assert((*equal)(0_c, 0_c), "");
	static_assert(!(*equal)(0_c, 1_c), "");
	
	static_assert(detail::equal_impl_std_ic{}(ic_c<0>, ic_c<0>), "");
	static_assert((*equal)(ic_c<0>, ic_c<0>), "");
	static_assert(!(*equal)(ic_c<0>, ic_c<1>), "");
	
	BOOST_CHECK(!(*not_equal)(a, 1));
	BOOST_CHECK((*not_equal)(a, 2));
	
	static_assert((*equal)(c, 3), "");
	static_assert(!(*equal)(c, 4), "");
	
	static_assert(!(*not_equal)(c, 3), "");
	static_assert((*not_equal)(c, 4), "");
	
	static_assert(!(*not_equal)(0_c, 0_c), "");
	static_assert((*not_equal)(0_c, 1_c), "");
	
	static_assert(!(*not_equal)(ic_c<0>, ic_c<0>), "");
	static_assert((*not_equal)(ic_c<0>, ic_c<1>), "");
	
	BOOST_CHECK((*less_equal)(a, b));
	BOOST_CHECK(!(*less_equal)(b, a));
	static_assert((*less_equal)(c, d), "");
	static_assert(!(*less_equal)(d, c), "");
	
	//TODO: further unit tests for other compare functions
	
	static_assert(hana::type_c<decltype( std::declval<int>()+std::declval<int>())> == hana::type_c<int>, "");
	static_assert(ic_c<1> + ic_c<2> == ic_c<3>, "");
	static_assert((*plus)(1, 2) == 3, "");
	
	static_assert((*divide)(hana::int_c<6>,  hana::int_c<3>) == hana::int_c<2>, "");
	static_assert((*divide)(hana::int_c<6>,  3) == 2, "");
	static_assert((*divide)(hana::uint_c<6>, hana::uint_c<3>) == hana::uint_c<2>, "");
	
	// NOTE: hana::integral_constant is converted implicitly to std::integral_constant
// 	static_assert( is_error_cause((*divide)(hana::int_c<6>, hana::uint_c<3>), error_cause::not_implemented{}), "");
	// NOTE: hana::integral_constant is converted implicitly to std::integral_constant
// 	static_assert( is_error_cause((*divide)(hana::int_c<1>, 2u), error_cause::not_implemented{}), "");
	
// 	static_assert(detail::plus_impl_hana_ic{}(1_c, 2ll) == 3, "");
	static_assert(detail::plus_impl_std_ic{}(1_c, 2ll) == 3, "");
	static_assert((*plus)(1_c, 2) == 3, "");
	static_assert((*plus)(1_c, 2_c) == 3_c, "");
	static_assert((*plus)(ic_c<1>, ic_c<2>) == ic_c<3>, "");
	
	std::integral_constant<std::size_t, 3> ic3a = detail::plus_impl_std_ic{}(ic_c<1>, ic_c<2>);
	std::integral_constant<std::size_t, 3> ic3b = (*plus)(ic_c<1>, ic_c<2>);
	
	static_assert(detail::plus_impl_std_ic{}(ic_c<1>, 2u) == 3, "");
	BOOST_CHECK((*plus)(ic_c<1>, 2u) == 3);
	std::integral_constant<std::size_t, 3> ic3c = detail::plus_impl_hana_ic{}(ic_c<1>, ic_c<2>);
	
	std::integral_constant<uint, 3> hc3a = (*plus)(hana::uint_c<0>, hana::uint_c<3>);
	
	static_assert((*minus)(1, 2) == -1, "");
	static_assert((*minus)(1_c, 2_c) == -1_c, "");
	static_assert((*multiply)(3, -4) == -12, "");
	static_assert((*multiply)(3_c, -4_c) == -12_c, "");
	static_assert((*divide)(6, 3) == 2, "");
	BOOST_TEST((*divide)(6, 3ul) == 2);
	static_assert((*divide)(6_c, 3_c) == 2_c, "");
	static_assert((*modulo)(7, 3) == 1, "");
	static_assert((*modulo)(7_c, 3_c) == 1_c, "");
	
	int e = 1;
	BOOST_ASSERT((*preincrement)(e) == 2);
	BOOST_ASSERT(e == 2);
	
	BOOST_ASSERT((*predecrement)(e) == 1);
	BOOST_ASSERT(e == 1);
	
	int f = 1;
	BOOST_ASSERT((*postincrement)(f) == 1);
	BOOST_ASSERT(f == 2);
	
	BOOST_ASSERT((*postdecrement)(f) == 2);
	BOOST_ASSERT(f == 1);
	
	static_assert((*not_)(true) == false, "");
	static_assert(detail::not__impl_std_op{}(true) == false, "");
	
	static_assert((*and_)(false, false) == false, "");
	static_assert((*and_)(false, true) == false, "");
	static_assert((*and_)(true, false) == false, "");
	static_assert((*and_)(true, true) == true, "");
	
	static_assert((*or_)(false, false) == false, "");
	static_assert((*or_)(false, true) == true, "");
	static_assert((*or_)(true, false) == true, "");
	static_assert((*or_)(true, true) == true, "");
	
	static_assert((*less)(
		std::array<int, 3>{{1,2,3}},
		std::array<int, 3>{{1,2,4}}
	), "");
	
	
	static_assert(detail::equal_impl_hana_pair{}(
		hana::make_pair(1,2),
		hana::pair<int,int>{1,2}
	), "");
	
	static_assert((*equal)(
		hana::make_pair(1,2),
		hana::pair<int,int>{1,2}
	), "");
	
	static_assert((*not_equal)(
		hana::pair<int,int>{1,2},
		hana::pair<int,int>{3,4}
	), "");
	
	// TODO: further unit tests for other operators functions
	detail::equal_impl_numeric_cast{}(1ul,1);
	BOOST_TEST((*equal)(1u, 1));
}

BOOST_AUTO_TEST_SUITE_END()
