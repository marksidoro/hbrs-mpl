/* Copyright (c) 2016 Jakob Meng, <jakobmeng@web.de>
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


#define BOOST_TEST_MODULE are_same_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <hbrs/mpl/detail/are_same.hpp>

BOOST_AUTO_TEST_SUITE(is_same_test)

BOOST_AUTO_TEST_CASE(is_same_test_1) {
	using namespace hbrs::mpl::detail;
	
	static_assert(are_same_v<int, int, int>, "");
	static_assert(!are_same_v<double, int>, "");
}

BOOST_AUTO_TEST_SUITE_END()
