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

#define BOOST_TEST_MODULE escsv_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <boost/hana/type.hpp>
#include <hbrs/mpl/dt/ctsav.hpp>
#include <hbrs/mpl/dt/rtsav.hpp>
#include <hbrs/mpl/dt/escsv.hpp>
#include <type_traits>

BOOST_AUTO_TEST_SUITE(escsv_test)

namespace detail {
	
	constexpr int 
	sum(int const* begin, int const* end, int partial)  {
		return (begin == end)
			? partial
			: sum(begin+1, end, partial+*begin);
	}
/* namespace detail */ }

BOOST_AUTO_TEST_CASE(escsv_test_1) {
	using namespace hbrs::mpl;
	static constexpr int a[] = { 1, 2, 3 };
	static constexpr int b[] = { 4, 5, 6 };
	constexpr int c[] = { 0xDEAD, 0xBEEF };
	
	static constexpr auto a_v = make_ctsav(a);
	static constexpr auto b_v = make_ctsav(b);
	
	static_assert(std::is_same<
		boost::hana::tag_of_t<decltype(make_escsv(a_v, b_v, make_ctsav(c)))>,
		hbrs::mpl::escsv_tag
	>::value, "");
	
	static constexpr auto cv_ab = make_escsv(a_v, b_v);
	
	static_assert(cv_ab.length() == 6, "");
	static_assert(cv_ab[1u] == 2, "");
	static_assert(cv_ab[4u] == 5, "");
	
	for(int i = 0; i < 6; ++i) {
		BOOST_CHECK(cv_ab[i] == i+1);
	}
	
	constexpr static auto cv_b = make_escsv(b_v, b_v);
	static_assert(cv_b.length() == 6, "");
	static_assert(cv_b[0u] == 4, "");
	static_assert(cv_b[1u] == 5, "");
	static_assert(cv_b[2u] == 6, "");
	static_assert(cv_b[3u] == 4, "");
	static_assert(cv_b[4u] == 5, "");
	static_assert(cv_b[5u] == 6, "");
	
	int d[3] = { 0 };
	int e[3] = { 0 };
	
	auto cv_d = make_ctsav(d);
	auto cv_e = make_ctsav(e);
	auto ev_de = make_escsv(cv_d, cv_e);
	
	// unsigned-signed-comparisons are always runtime-only because they require numeric_cast<> which throws exceptions.
	BOOST_TEST(   make_escsv(hana::make_tuple(1,2,3), hana::make_tuple(4,5,6))[5] == 6);
	BOOST_TEST(   make_escsv(hana::make_tuple(1,2,3), std::make_tuple(4,5))[4] == 5);
	
	static_assert(make_escsv(hana::make_tuple(1,2,3), hana::make_tuple(4,5,6))[5u] == 6, "");
	static_assert(make_escsv(hana::make_tuple(1,2,3), std::make_tuple(4,5))[4u] == 5, "");
	
	for(std::size_t i = 0; i < ev_de.length(); ++i) {
		ev_de[i] = i+1;
	}
	
	for(int i = 0; i < 3; ++i) {
		BOOST_CHECK(d[i] == i+1);
	}
	
	for(int i = 0; i < 3; ++i) {
		BOOST_CHECK(e[i] == i+1+3);
	}
}

BOOST_AUTO_TEST_SUITE_END()
