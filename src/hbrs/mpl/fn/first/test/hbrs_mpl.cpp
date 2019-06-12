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


#define BOOST_TEST_MODULE first_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/fn/first.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/equal.hpp>
#include <tuple>
#include <utility>

BOOST_AUTO_TEST_SUITE(first_test)

BOOST_AUTO_TEST_CASE(first_test_1) {
	using namespace hbrs::mpl;
	using namespace hana::literals;
	
	static_assert((*first)(hana::make_pair(42_c, 666_c)) == 42_c, "");
	static_assert(detail::first_impl_std_pair{}(std::make_pair(42_c, 666_c)) == 42_c, "");
	static_assert((*first)(std::make_pair(42_c, 666_c)) == 42_c, "");
}

BOOST_AUTO_TEST_SUITE_END()
