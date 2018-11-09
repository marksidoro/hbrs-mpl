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


#define BOOST_TEST_MODULE uscsv_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <boost/hana/type.hpp>
#include <hbrs/mpl/dt/ctsav.hpp>
#include <hbrs/mpl/dt/rtsav.hpp>
#include <hbrs/mpl/dt/uscsv.hpp>
#include <type_traits>

BOOST_AUTO_TEST_SUITE(uscsv_test)

namespace detail {
	
	constexpr int 
	sum(int const* begin, int const* end, int partial)  {
		return (begin == end)
			? partial
			: sum(begin+1, end, partial+*begin);
	}
	
/* namespace detail */ }

BOOST_AUTO_TEST_CASE(uscsv_test_1) {
	using namespace hbrs::mpl;
	
	//compile-time-size arrays
	constexpr int a[] = { 1, 2, 3 };
	static constexpr int b[] = { 4, 5, 6 };
	static constexpr int c[] = { 0xDEAD, 0xBEEF };
	
	//runtime-size arrays
	int d_length = 2;
	auto d = std::make_unique<int[]>(d_length);
	d[0] = 0xDEAD;
	d[1] = 0xBEEF;
	
	int e[] = { 1, 2, 3 };
	int e_length = sizeof(e)/sizeof(e[0]);
	
	int f[] = { 4, 5, 6 };
	int f_length = sizeof(f)/sizeof(f[0]);
	
	//views
	auto a_cv = make_ctsav(a);
	constexpr static auto b_cv = make_ctsav(b);
	constexpr auto b_cv2 = make_ctsav(b);
	
	static_assert(std::is_same<
		decltype(b_cv),
		const ctsav<const int, 3>
	>::value, "");
	
	static_assert(b_cv.length() == 3 && b_cv2.length() == 3, "");
	
	auto const d_rv = make_rtsav(d.get(), d_length);
	auto e_rv = make_rtsav(std::addressof(e[0]), e_length);
	
	static_assert(std::is_same<
		boost::hana::tag_of_t<decltype(make_uscsv(a_cv))>,
		uscsv_tag
	>::value, "");
	
	typedef decltype( make_uscsv(a_cv, b_cv, make_ctsav(c)) ) uscsv_ab;
	uscsv_ab cv_abc = make_uscsv(a_cv, b_cv, make_ctsav(c));
	
	BOOST_CHECK(cv_abc.length() == 8);
	
	static_assert(make_uscsv(b_cv).length() == 3, "");
	
	constexpr static auto cv_b = make_uscsv( b_cv ); /* requires b and b_cv to be global (e.g. static) constexpr variables! */
	constexpr auto cv_b2 = make_uscsv( make_ctsav(b) ); /* requires b to be a global (e.g. static) constexpr variable! */
	typedef std::decay_t< decltype(b_cv2) > b_cv2_type;
	constexpr auto cv_b3 = make_uscsv( b_cv2_type{b_cv2} ); /* copy construct, NOTE: the resulting ctsav object will be saved in uscsv object! */
	
	static_assert(cv_b.length() == 3, "");
	static_assert(cv_b2.length() == 3, "");
	static_assert(cv_b3.length() == 3, "");
	
	static_assert(make_uscsv(make_ctsav(c)).length() == 2, "");
	
// 	using namespace boost::hana::literals;
// 	constexpr static auto e_b = cv_b[0_c];
	constexpr static auto e_b = cv_b[0u];
	static_assert(e_b == 4,"");
	
	static_assert(make_uscsv(make_ctsav(c))[0u] == 0xDEAD, "");
	static_assert(make_uscsv(make_ctsav(c))[1u] == 0xBEEF, "");
	
	static_assert(cv_b[0u] == 4, "");
	static_assert(cv_b[1u] == 5, "");
	static_assert(cv_b[2u] == 6, "");
	
	constexpr static auto cv_bbb = make_uscsv( b_cv, b_cv, b_cv );
	
	static_assert(cv_bbb.length() == 9, "");
	static_assert(cv_bbb[0u] == 4, "");
	static_assert(cv_bbb[3u] == 4, "");
	static_assert(cv_bbb[6u] == 4, "");
	static_assert(cv_bbb[8u] == 6, "");
	
	static_assert(cv_bbb[hana::size_c<8>] == 6, "");
	//static_assert(cv_bbb[hana::int_c<8>] == 6, "");
	
	
	auto rv_d = make_uscsv(d_rv);
	BOOST_CHECK(rv_d.length() == 2);
	BOOST_CHECK(rv_d[0u] == 0xDEAD);
	BOOST_CHECK(rv_d[1u] == 0xBEEF);
	
	auto rv_dd = make_uscsv(d_rv, d_rv);
	BOOST_CHECK(rv_dd.length() == 4);
	BOOST_CHECK(rv_dd[2u] == 0xDEAD);
	BOOST_CHECK(rv_dd[3u] == 0xBEEF);
	
	auto rv_e = make_uscsv(e_rv);
	BOOST_CHECK(rv_e.length() == e_rv.length());
	BOOST_CHECK(rv_e[0u] == 1);
	BOOST_CHECK(rv_e[1u] == 2);
	
	auto const& e_crv = e_rv;
	auto rv_def = make_uscsv(d_rv, e_crv, make_rtsav(std::addressof(f[0]), f_length));
	BOOST_CHECK(rv_def.length() == 8);
	
	BOOST_CHECK(rv_def[7u] == 6);
	
	/*
	for(uscsv_ab::length_type i = 0; i < cv_ab.length(); ++i) {
		static_assert(cv_ab[i] == i+1, "");
	}
	
	int d[] = { 1 };
	int e[] = { 2, 3 };
	int f[] = { 4, 5, 6 };
	/*
	typedef decltype( make_uscsv(a_v, b_v, make_ctsav(c)) ) uscsv_abc;
	uscsv_abc cv_abc = make_uscsv(a_v, b_v, make_ctsav(c));
	
	for(uscsv_abc::length_type i = 0; i < cv_abc.length(); ++i) {
		cv_abc[i] = i+1;
	}
	*/
}

BOOST_AUTO_TEST_SUITE_END()
