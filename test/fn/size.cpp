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

#define BOOST_TEST_MODULE size_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/dt/ctsav.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/ext/std/tuple.hpp>
#include <tuple>
#include <vector>

#include <iostream>

BOOST_AUTO_TEST_SUITE(size_test)

BOOST_AUTO_TEST_CASE(size_array) {
	using namespace hbrs::mpl;
	
	static constexpr auto a = std::array<int, 3>{{1,2,3}};
	static_assert((*size)(a) == 3ul, "");
}

BOOST_AUTO_TEST_CASE(size_vector) {
	using namespace hbrs::mpl;
	
	auto f1 = [](auto && v) {
		auto sz = (*size)(HBRS_MPL_FWD(v));
		BOOST_TEST(sz == 3);
		return HBRS_MPL_FWD(v);
	};
	
	auto f2 = [](auto && v) {
		auto sz = (*size)(v);
		BOOST_TEST(sz == 3);
		return HBRS_MPL_FWD(v);
	};
	
	BOOST_TEST(f1(std::vector<int>{1,2,3}).empty() == true);
	BOOST_TEST(f2(std::vector<int>{1,2,3}).empty() == false);
}

BOOST_AUTO_TEST_CASE(size_test_1) {
	using namespace hbrs::mpl;
	
	constexpr static int a[] = { 1, 2, 3 };
	constexpr auto const a_v = make_ctsav(a);
	
	int b[] = { 1, 2, 3 };
	auto b_v = make_ctsav(b);
	
	
	static_assert((*size)(a_v) == 3, "");
	BOOST_CHECK((*size)(b_v) == 3);
	
	static_assert((*size)(std::tuple<int, double, float>{}) == std::integral_constant<std::size_t, 3>{}, "");
	static_assert((*size)(std::tuple<int, double, float>{}) == hana::size_c<3>, "");
	static_assert((*size)(hana::make_tuple(1, 2.0, 'c')) == hana::size_c<3>, "");
}

BOOST_AUTO_TEST_SUITE_END()
