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


#define BOOST_TEST_MODULE ctsav_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <boost/hana/type.hpp>
#include <hbrs/mpl/dt/ctsav.hpp>
#include <type_traits>

BOOST_AUTO_TEST_SUITE(ctsav_test)

namespace detail {
	
	constexpr int 
	sum(int const* begin, int const* end, int partial)  {
		return (begin == end)
			? partial
			: sum(begin+1, end, partial+*begin);
	}
/* namespace detail */ }

BOOST_AUTO_TEST_CASE(ctsav_test_1) {
	using namespace hbrs::mpl;
	int a[] = { 1, 2, 3 };
	constexpr static int b[] = { 4, 5, 6 };
	constexpr int c[] = { 0xDEAD, 0xBEEF };
	
	static_assert(std::is_same<
		boost::hana::tag_of_t<decltype(make_ctsav(a))>,
		ctsav_tag
	>::value, "");
	
	ctsav<int,3> a_v = make_ctsav(a);
	BOOST_CHECK(detail::sum(a_v.begin(), a_v.end(), 0) == 6);
	BOOST_CHECK(a_v[0] == 1);
	
	constexpr auto const b_v = make_ctsav(b);
	BOOST_CHECK(detail::sum(b_v.begin(), b_v.end(), 0) == 15);
	static_assert(detail::sum(b_v.begin(), b_v.end(), 0) == 15, "");
	BOOST_CHECK(b_v[2] == 6);
	
	static_assert(std::is_same<
		decltype(make_ctsav(b)),
		ctsav<const int, 3>
	>::value, "");
	
	BOOST_CHECK(make_ctsav(c).length() == 2);
	BOOST_CHECK(make_ctsav(c)[1] == 0xBEEF);
}

BOOST_AUTO_TEST_SUITE_END()
