/* Copyright (c) 2016-2019 Jakob Meng, <jakobmeng@web.de>
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

#define BOOST_TEST_MODULE index_of_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include <hbrs/mpl/detail/index_of.hpp>

BOOST_AUTO_TEST_SUITE(index_of_test)

BOOST_AUTO_TEST_CASE(index_of_test_1) {
	using namespace hbrs::mpl;
	
	static_assert(
		detail::index_of(hana::make_tuple(hana::int_c<3>, hana::type_c<bool>), hana::type_c<bool>) == hana::size_c<1>
		, "");
}

BOOST_AUTO_TEST_SUITE_END()
