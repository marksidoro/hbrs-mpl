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

#include <hbrs/mpl/fn/iterate.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/plus.hpp>
#include <boost/hana/equal.hpp>

#define BOOST_TEST_MODULE iterate_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

namespace utf = boost::unit_test;
namespace tt = boost::test_tools;

BOOST_AUTO_TEST_SUITE(iterate_test)

BOOST_AUTO_TEST_CASE(iterate_test) {
	using namespace hbrs::mpl;
	static constexpr auto add2 = [](auto && n) { return HBRS_MPL_FWD(n) + hana::size_c<2>; };
	
	BOOST_TEST(    (*iterate)(add2,              5u )(             1337u ) ==              1347u );
	static_assert( (*iterate)(add2,              5u )(             1337u ) ==              1347u , "");
	static_assert( (*iterate)(add2, hana::size_c<5u>)(hana::size_c<1337u>) == hana::size_c<1347u>, "");
}

BOOST_AUTO_TEST_SUITE_END()

