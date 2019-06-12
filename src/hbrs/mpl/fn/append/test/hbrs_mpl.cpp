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


#define BOOST_TEST_MODULE append_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/append.hpp>
#include <hbrs/mpl/fn/size.hpp>

#include <boost/hana/type.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/equal.hpp>
#include <tuple>
#include <vector>

BOOST_AUTO_TEST_SUITE(append_test)

BOOST_AUTO_TEST_CASE(append_test_1) {
	using namespace hbrs::mpl;
	
	static_assert(
		hana::make_type( (*append)(hana::make_tuple(1, 2.0), 'c') ) ==
		hana::make_type(                 hana::make_tuple(1, 2.0,  'c')  )
	, "");
	
	static constexpr std::tuple<int, double> t1{1, 2.0};
	static constexpr std::tuple<int, double, char> t2{1, 2.0, 'c'};
	//static constexpr auto t3 = detail::append_impl_std_tuple{}(t1, 'c');
	//static constexpr auto t3 = hana::append(t1, 'c');
	static constexpr auto t3 = (*append)(t1, 'c');
	
	static_assert((*size)(t2) == (*size)(t3), "");
	static_assert(hana::make_type(t2) == hana::make_type(t3), "");
	
	std::vector<int> v1{1337, 42, 666};
	std::vector<int> v2{1337, 42, 666, 4711};
	auto v3 = (*append)(v1, 4711);
	
	BOOST_CHECK((*size)(v2) == (*size)(v3));
	
	
	for(std::size_t i = 0; i < (*size)(v3); ++i) {
		BOOST_CHECK((*at)(v2, i) == (*at)(v3, i));
	}
}

BOOST_AUTO_TEST_SUITE_END()
