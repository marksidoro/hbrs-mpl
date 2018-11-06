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

#define BOOST_TEST_MODULE sms_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/dt/sms.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/dt/rtsav.hpp>
#include <hbrs/mpl/dt/ctsam.hpp>
#include <hbrs/mpl/dt/rtsam.hpp>
#include <array>
#include <vector>

#include "../data.hpp"

BOOST_AUTO_TEST_SUITE(sms_test)

BOOST_AUTO_TEST_CASE(ctsam_make_size_at) {
	using namespace hbrs::mpl;
	
	static constexpr auto ca = std::array<double, 6>{{
		1., 2., 3.,
		4., 5., 6.
	}};
	static constexpr auto cm_sz = make_matrix_size(hana::size_c<2>, hana::size_c<3>);
	static constexpr auto cm = make_ctsam(ca, cm_sz, row_major_c);
	static constexpr auto cms = make_sms(cm);
	static_assert((*size)(cms) == cm_sz.m() * cm_sz.n(), "");
	for(std::size_t i = 0; i < (*size)(cms); ++i) {
		BOOST_CHECK((*at)(cms, i) == (*at)(ca, i));
	}
}

BOOST_AUTO_TEST_CASE(rtsam_make_size_at) {
	using namespace hbrs::mpl;
	
	auto ra = std::vector<double> {{
		1., 2., 3.,
		4., 5., 6.
	}};
	auto rm_sz = matrix_size<std::size_t, std::size_t>{2u,3u};
	auto rm = make_rtsam(ra, rm_sz, row_major_c);
	auto rms = make_sms(rm);
	BOOST_CHECK((*size)(rms) == rm_sz.m() * rm_sz.n());
	for(std::size_t i = 0; i < (*size)(rms); ++i) {
		BOOST_CHECK((*at)(rms, i) == (*at)(ra, i));
	}
}

BOOST_AUTO_TEST_CASE(sm_make_size_at) {
	using namespace hbrs::mpl;
	static constexpr auto ca = std::array<double, 6>{{
		1., 2., 3.,
		4., 5., 6.
	}};
	static constexpr auto cav = make_rtsav(ca.data(), ca.size());
	
	static constexpr auto cm_sz = make_matrix_size(hana::size_c<2>, hana::size_c<3>);
	static constexpr auto cm = make_sm(cav, cm_sz, row_major_c);
	static constexpr auto cms = make_sms(cm);
	
	BOOST_CHECK((*size)(cms) == ca.size());
	for(std::size_t i = 0; i < (*size)(cms); ++i) {
		BOOST_CHECK((*at)(cms, i) == (*at)(ca, i));
	}
}

BOOST_AUTO_TEST_SUITE_END()
