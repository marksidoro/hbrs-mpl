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


#define BOOST_TEST_MODULE insert_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/fn/insert.hpp>
#include <hbrs/mpl/fn/where.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/pair.hpp>
#include <map>

BOOST_AUTO_TEST_SUITE(insert_test)

struct flag1 {};

BOOST_AUTO_TEST_CASE(insert_test_1) {
	using namespace hbrs::mpl;
	
	detail::insert_impl_hana_map{}(
		hana::make_map(
			hana::make_pair(hana::size_c<1>, 666)
		),
		hana::make_pair(hana::size_c<2>, hana::size_c<1337>)
	);
	
	static constexpr auto m1 = (*insert)(
		hana::make_map(
			hana::make_pair(hana::size_c<1>, 666)
		),
		hana::make_pair(hana::size_c<2>, hana::size_c<1337>)
	);
	
	hana::size_t<1337> v0 = (*where)(m1, hana::size_c<2>).value();
	
	static_assert(
		(*where)(
			insert(
				hana::make_map(
					hana::make_pair(hana::size_c<1>, 666)
				),
				hana::make_pair(hana::size_c<2>, hana::size_c<1337>)
			),
            hana::size_c<2>
		).value() == hana::size_c<1337>, "");
	
	
	typedef std::map<char, int> map_t;
	
	map_t const m0{ { 'a', 1337 } };
	BOOST_CHECK(detail::insert_impl_std_map{}(map_t{}, std::make_pair('a', 1337)) == m0);
	BOOST_CHECK((*insert)(map_t{}, std::make_pair('a', 1337)) == m0);
	
	map_t const m2;
	auto m3 = detail::insert_impl_std_map{}(m2, std::make_pair('a', 1337));
	BOOST_CHECK(m3 == m0);
	auto m4 = (*insert)(m2, std::make_pair('a', 1337));
	BOOST_CHECK(m4 == m0);
}

BOOST_AUTO_TEST_SUITE_END()
