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


#define BOOST_TEST_MODULE rtsav_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <boost/hana/type.hpp>
#include <hbrs/mpl/dt/rtsav.hpp>
#include <type_traits>
#include <memory>

BOOST_AUTO_TEST_SUITE(rtsav_test)

namespace detail {
	
	constexpr int 
	sum(int const* begin, int const* end, int partial)  {
		return (begin == end)
			? partial
			: sum(begin+1, end, partial+*begin);
	}
/* namespace detail */ }

BOOST_AUTO_TEST_CASE(rtsav_test_1) {
	using namespace hbrs::mpl;
	int a[] = { 1, 2, 3 };
	int * ap = a;
	constexpr std::size_t a_length = sizeof(a)/sizeof(a[0]);
	static constexpr int b[] = { 4, 5, 6 };
	static constexpr int const * bp = b;
	static constexpr std::size_t b_length = sizeof(b)/sizeof(b[0]);
	std::size_t c_length = 2;
	auto c = std::make_unique<int[]>(c_length);
	c[0] = 0xDEAD;
	c[1] = 0xBEEF;
	
	int const * d = a;
	std::size_t d_length = a_length;
	
	int ev{1337};
	int* e{&ev};
	std::size_t const e_length{1};
	
	static_assert(std::is_same<
		boost::hana::tag_of_t<decltype(make_rtsav(ap, a_length))>,
		rtsav_tag
	>::value, "");
	
	auto a_v = make_rtsav(ap, a_length);
	BOOST_CHECK(detail::sum(a_v.begin(), a_v.end(), 0) == 6);
	BOOST_CHECK(a_v[0] == 1);
	
	static constexpr auto b_v = make_rtsav(bp, b_length);
	static_assert(detail::sum(b_v.begin(), b_v.end(), 0) == 15, "");
	static_assert(b_v[2] == 6, "");
	
	auto const c_v = make_rtsav(c.get(), c_length);
	BOOST_CHECK(c_v.length() == c_length);
	BOOST_CHECK(c_v[1] == 0xBEEF);
	
	auto const d_v = make_rtsav(d, d_length);
	BOOST_CHECK(d_v.length() == d_length);
	BOOST_CHECK(d_v[1] == 2);
	
	auto const e_v = make_rtsav(e, e_length);
	BOOST_CHECK(e_v.length() == e_length);
	BOOST_CHECK(e_v[0] == 1337);
}

BOOST_AUTO_TEST_SUITE_END()
