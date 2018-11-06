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


#define BOOST_TEST_MODULE contains_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/fn/contains.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/equal.hpp>
#include <tuple>
#include <vector>

BOOST_AUTO_TEST_SUITE(contains_test)

BOOST_AUTO_TEST_CASE(contains_test_1) {
	using namespace hbrs::mpl;
	using namespace hana::literals;
	
	static_assert((*contains)(std::make_tuple(1_c, 2_c, 3_c), 1_c) == hana::true_c, "");
	static_assert((*contains)(std::make_tuple(1_c, 2_c, 3_c), 4_c) == hana::false_c, "");
	
	static_assert((*contains)(hana::make_tuple(1_c, 2_c, 3_c), 1_c), "");
	static_assert(!(*contains)(hana::make_tuple(1_c, 2_c, 3_c), 4_c), "");
	
	std::vector<int> v1{1337, 42, 666};
	
	BOOST_CHECK((*contains)(v1, 666));
	BOOST_CHECK(!(*contains)(v1, 4711));
	
}

BOOST_AUTO_TEST_SUITE_END()
