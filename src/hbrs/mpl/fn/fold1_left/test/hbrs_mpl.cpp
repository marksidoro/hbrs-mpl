/* Copyright (c) 2018 Jakob Meng, <jakobmeng@web.de>
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


#define BOOST_TEST_MODULE fold1_left_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/fn/fold1_left.hpp>
#include <hbrs/mpl/fn/plus.hpp>
#include <hbrs/mpl/fn/equal.hpp>
#include <hbrs/mpl/dt/zas.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/first.hpp>
#include <boost/hana/second.hpp>
#include <boost/hana/equal.hpp>
#include <tuple>
#include <vector>

BOOST_AUTO_TEST_SUITE(fold1_left_test)

BOOST_AUTO_TEST_CASE(tuple_vector) {
	using namespace hbrs::mpl;
	using namespace hana::literals;
	
	BOOST_TEST(detail::fold1_left_impl_std{}(std::vector<int>{1,2,3}, *plus) == 6);
	BOOST_TEST(   (*fold1_left)(std::array<int, 3>{{1,2,3}}, *plus) == 6  );
	BOOST_TEST(   (*fold1_left)(std::array<int, 3>{{1,2,3}}, *plus) == 6  );
	static_assert((*fold1_left)(std::array<int, 3>{{1,2,3}}, *plus) == 6, "");
	static_assert((*fold1_left)(std::make_tuple(1_c, 2_c, 3_c), *plus) == 6_c, "");
	static_assert((*fold1_left)(hana::make_tuple(1_c, 2_c, 3_c), *plus) == 6_c, "");
	static_assert((*fold1_left)(hana::make_tuple(1, 2, 3), *plus) == 6, "");

	BOOST_TEST(  (*fold1_left)(std::vector<int>{1,2,3}, *plus) == 6  );
	
	struct plus_{ int operator()(int a, int b) { return a+b; } };
	BOOST_TEST(  (*fold1_left)(std::vector<int>{1,2,3}, plus_{}) == 6);
	
	static constexpr auto z1 = make_zas(std::make_tuple(1,2,3), std::make_tuple(4,5,6));
	static constexpr hana::pair<int,int> p1 = (*fold1_left)(
		z1, 
		[](auto const& p1, auto const& p2) {
			return hana::make_pair(hana::first(p1) + hana::first(p2), hana::second(p1) + hana::second(p2));
		}
	);
	static_assert(p1 == hana::make_pair(6, 15), "");
}

BOOST_AUTO_TEST_SUITE_END()
