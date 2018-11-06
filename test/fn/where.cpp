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


#define BOOST_TEST_MODULE where_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/fn/where.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <map>

BOOST_AUTO_TEST_SUITE(where_test)

struct flag1{};

BOOST_AUTO_TEST_CASE(where_test_1) {
	using namespace hbrs::mpl;
	
	static_assert(
		(*where)(
			hana::make_map(
				hana::make_pair(hana::make_type(flag1{}), hana::size_c<1337>)
			),
			hana::make_type(flag1{})
		).value()
		== hana::size_c<1337>, "");
	
	static_assert(
		(*where)(
			hana::make_map(
				hana::make_pair(hana::make_type(flag1{}), hana::size_c<1337>)
			),
			hana::make_type(1)
		)
		== hana::nothing, "");
	
	std::map<char, int> m0;
	m0.insert(std::make_pair('c', 1337));
	BOOST_CHECK((*where)(m0, 'c') == 1337);
}

BOOST_AUTO_TEST_SUITE_END()
