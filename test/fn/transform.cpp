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


#define BOOST_TEST_MODULE transform_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/dt/zas.hpp>
#include <hbrs/mpl/fn/transform.hpp>
#include <hbrs/mpl/fn/equal.hpp>
#include <boost/hana/integral_constant.hpp>

#include <boost/hana/tuple.hpp>
#include <boost/hana/first.hpp>
#include <boost/hana/second.hpp>
#include <array>
#include <tuple>
#include <vector>

BOOST_AUTO_TEST_SUITE(transform_test)

BOOST_AUTO_TEST_CASE(tuple_vector) {
	using namespace hbrs::mpl;
	using namespace hana::literals;
	
	static constexpr auto inc = [](auto && nr) { return HBRS_MPL_FWD(nr)+hana::int_c<1>; };
	
	BOOST_TEST((*transform)(std::vector<int>{1,2,3}, inc) == (std::vector<int>{2,3,4})  );
	BOOST_TEST((*transform)(std::array<int, 3>{{1,2,3}}, inc) == (std::array<int, 3>{{2,3,4}}) );
	
	static_assert((*equal)(
		transform(std::array<int, 3>{{1,2,3}}, inc),
		std::array<int, 3>{{2,3,4}}
	), "");
	static_assert((*transform)(std::make_tuple(1_c, 2_c, 3_c), inc) == std::make_tuple(2_c, 3_c, 4_c), "");
	static_assert((*transform)(hana::make_tuple(1_c, 2_c, 3_c), inc) == hana::make_tuple(2_c, 3_c, 4_c), "");
	static_assert((*transform)(hana::make_tuple(1, 2, 3), inc) == hana::make_tuple(2, 3, 4), "");
	
	static constexpr auto z1 = make_zas(std::array<int, 3>{1,2,3}, std::array<int, 3>{4,5,6});
	auto z2 = make_zas(std::vector<int>{{1,2,3}}, boost::irange(4, 7));
	
	static constexpr auto add = [](auto pair) {
		return hana::first(pair) + hana::second(pair);
	};
	
// 	detail::transform_impl_zas_std_array_vector_irange{}
	static_assert(
		(*equal)(
			transform(z1, add),
			std::array<int, 3>{5,7,9}
		), "");
	
	
	BOOST_TEST(
		(*transform)(z2, add)
		== (std::vector<int>{{5,7,9}}));
}

BOOST_AUTO_TEST_SUITE_END()
