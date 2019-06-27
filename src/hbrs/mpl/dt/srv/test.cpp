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

#define BOOST_TEST_MODULE srv_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/dt/srv.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/transpose.hpp>
#include <hbrs/mpl/dt/rtsav.hpp>
#include <hbrs/mpl/dt/ctsav.hpp>

#include <hbrs/mpl/detail/test.hpp>

BOOST_AUTO_TEST_SUITE(srv_test)

BOOST_AUTO_TEST_CASE(ctsav_make_size) {
	using namespace hbrs::mpl;
	
	static constexpr auto cav_g = make_ctsav(detail::mat_g);
	static constexpr auto crv_g_sz = hana::size_c<detail::mat_g_m*detail::mat_g_n>;
	static constexpr auto crv_g = make_srv(cav_g);
	static_assert((*size)(crv_g) == crv_g_sz, "");
	static_assert(detail::mat_g_m*detail::mat_g_n == crv_g_sz, "");
}

BOOST_AUTO_TEST_CASE(rtsav_make_size_at) {
	using namespace hbrs::mpl;
	
	auto ra_g = detail::mat_g;
	auto ra_g_length = detail::mat_g_m * detail::mat_g_n;
	auto rav_g = make_rtsav(ra_g, ra_g_length);
	
	auto rrv_g = srv< hbrs::mpl::rtsav<double const> >{rav_g};
	make_srv(rav_g);
	BOOST_CHECK((*size)(rrv_g) == detail::mat_g_m * detail::mat_g_n);
	BOOST_CHECK(detail::mat_g_m*detail::mat_g_n == (*size)(rrv_g));
	for(std::size_t i = 0; i < ra_g_length; ++i) {
		BOOST_CHECK(ra_g[i] == rrv_g[i]);
		BOOST_CHECK(ra_g[i] == (*at)(rrv_g, i));
	}
}

BOOST_AUTO_TEST_CASE(vector_size) {
	using namespace hbrs::mpl;
	
	auto const v1 = std::vector<int>{{1,2,3,4,5,6}};
	auto const v1_sz = (*size)(v1);
	
	auto const rv1 = make_srv(std::vector<int>{{1,2,3,4,5,6}});
	auto rv2 = make_srv(std::vector<int>(6));
	
	auto rv1_sz = (*size)(rv1);
	auto rv2_sz = (*size)(rv2);
	
	BOOST_TEST(v1_sz == rv1_sz);
	BOOST_TEST(v1_sz == rv2_sz);
	
	for(std::size_t i = 0; i < rv2_sz; ++i) {
		(*at)(rv2, i) = i+1;
	}
	
	for(std::size_t i = 0; i < v1_sz; ++i) {
		BOOST_TEST((*at)(rv1, i) == (*at)(v1, i));
		BOOST_TEST((*at)(rv2, i) == (*at)(v1, i));
	}
}

BOOST_AUTO_TEST_CASE(vector_transpose) {
	using namespace hbrs::mpl;
	
	srv<std::vector<int>> const cv1 = make_srv(std::vector<int>{{1,2,3,4,5,6}});
	scv<srv<std::vector<int>>> const rv1 = (*transpose)(cv1);
	
	auto rv1_sz = (*size)(rv1);
	auto cv1_sz = (*size)(cv1);
	
	BOOST_TEST(cv1_sz == rv1_sz);
	
	for(std::size_t i = 0; i < rv1_sz; ++i) {
		BOOST_TEST((*at)(rv1, i) == (*at)(cv1, i));
	}
}

BOOST_AUTO_TEST_SUITE_END()
