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


#define BOOST_TEST_MODULE signum_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/fn/signum.hpp>
#include <hbrs/mpl/fn/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/equal.hpp>
#include <tuple>
#include <vector>
#include <array>

BOOST_AUTO_TEST_SUITE(signum_test)

BOOST_AUTO_TEST_CASE(array_tuple_vector) {
	using namespace hbrs::mpl;
	using namespace hana::literals;
	
	static_assert((*signum)(2) == 1, "");
	static_assert((*signum)(-2) == -1, "");
	static_assert((*signum)(0) == 0, "");
	
	static_assert((*signum)(2_c) == 1_c, "");
	static_assert((*signum)(-2_c) == -1_c, "");
	static_assert((*signum)(0_c) == 0_c, "");
	
	static_assert((*equal)(
		signum(std::array<int, 3>{{2,-2,0}}),
		std::array<int, 3>{{1,-1,0}}
	), "");
	BOOST_TEST((*signum)(std::vector<int>{2,-2,0}) == (std::vector<int>{1,-1,0}));
	static_assert((*signum)(std::make_tuple(2_c, -2_c, 0_c)) == std::make_tuple(1_c, -1_c, 0_c), "");
}

BOOST_AUTO_TEST_SUITE_END()
