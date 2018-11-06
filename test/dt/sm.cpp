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

#define BOOST_TEST_MODULE sm_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/dt/sm.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>
#include <hbrs/mpl/dt/rtsav.hpp>
#include <hbrs/mpl/dt/ctsav.hpp>
#include <hbrs/mpl/dt/srv.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/at.hpp>

#include <iostream>
#include "../data.hpp"

BOOST_AUTO_TEST_SUITE(sm_test)

BOOST_AUTO_TEST_CASE(sms_test_1) {
	using namespace hbrs::mpl;
	
	static constexpr matrix_size<unsigned long, long> sz1{ 
		matrix_size<unsigned int, int>{0u,0}
	};
	
	static constexpr matrix_size<unsigned int, int> sz2{0u,0};
	static constexpr matrix_size<unsigned long, long> sz3{sz2};
	
	static constexpr auto ca_g = test::mat_g;
	static constexpr auto cav_g = make_ctsav(test::mat_g);
	static constexpr auto cm_g_sz = make_matrix_size(hana::size_c<test::mat_g_m>, hana::size_c<test::mat_g_n>);
	static constexpr auto cm_g = make_sm(cav_g, cm_g_sz, row_major_c);
	
	static_assert((*size)(cm_g) == cm_g_sz, "");
	static constexpr auto m_cm_g = (*m)(size(cm_g));
	static constexpr auto n_cm_g = (*n)(size(cm_g));
	
	static_assert(m_cm_g == test::mat_g_m, "");
	static_assert(n_cm_g == test::mat_g_n, "");
#define _HBRS_MPL_TEST1(i,j)                                                                                           \
	static_assert(ca_g[i * test::mat_g_n + j] == cav_g[i * test::mat_g_n + j], "");                                    \
	static_assert(ca_g[i * test::mat_g_n + j] == cm_g[hana::size_c<i>][hana::size_c<j>], "");                          \
	static_assert(cm_g[hana::size_c<i>][hana::size_c<j>] == (*at)(cm_g, make_matrix_index(hana::size_c<i>,hana::size_c<j>)), ""); \
	/*static_assert(ca_g[i * test::mat_g_n + j] == (*at)(cm_g, make_matrix_index(i,j)), "");*/
	
	_HBRS_MPL_TEST1( 0,0) _HBRS_MPL_TEST1( 0,1) _HBRS_MPL_TEST1( 0,2) _HBRS_MPL_TEST1( 0,3)
	_HBRS_MPL_TEST1( 1,0) _HBRS_MPL_TEST1( 1,1) _HBRS_MPL_TEST1( 1,2) _HBRS_MPL_TEST1( 1,3)
	_HBRS_MPL_TEST1( 2,0) _HBRS_MPL_TEST1( 2,1) _HBRS_MPL_TEST1( 2,2) _HBRS_MPL_TEST1( 2,3)
	_HBRS_MPL_TEST1( 3,0) _HBRS_MPL_TEST1( 3,1) _HBRS_MPL_TEST1( 3,2) _HBRS_MPL_TEST1( 3,3)
	_HBRS_MPL_TEST1( 4,0) _HBRS_MPL_TEST1( 4,1) _HBRS_MPL_TEST1( 4,2) _HBRS_MPL_TEST1( 4,3)
	_HBRS_MPL_TEST1( 5,0) _HBRS_MPL_TEST1( 5,1) _HBRS_MPL_TEST1( 5,2) _HBRS_MPL_TEST1( 5,3)
	_HBRS_MPL_TEST1( 6,0) _HBRS_MPL_TEST1( 6,1) _HBRS_MPL_TEST1( 6,2) _HBRS_MPL_TEST1( 6,3)
	_HBRS_MPL_TEST1( 7,0) _HBRS_MPL_TEST1( 7,1) _HBRS_MPL_TEST1( 7,2) _HBRS_MPL_TEST1( 7,3)
	_HBRS_MPL_TEST1( 8,0) _HBRS_MPL_TEST1( 8,1) _HBRS_MPL_TEST1( 8,2) _HBRS_MPL_TEST1( 8,3)
	_HBRS_MPL_TEST1( 9,0) _HBRS_MPL_TEST1( 9,1) _HBRS_MPL_TEST1( 9,2) _HBRS_MPL_TEST1( 9,3)
	_HBRS_MPL_TEST1(10,0) _HBRS_MPL_TEST1(10,1) _HBRS_MPL_TEST1(10,2) _HBRS_MPL_TEST1(10,3)
	_HBRS_MPL_TEST1(11,0) _HBRS_MPL_TEST1(11,1) _HBRS_MPL_TEST1(11,2) _HBRS_MPL_TEST1(11,3)
	_HBRS_MPL_TEST1(12,0) _HBRS_MPL_TEST1(12,1) _HBRS_MPL_TEST1(12,2) _HBRS_MPL_TEST1(12,3)
	
#undef _HBRS_MPL_TEST1

	auto ra_g = test::mat_g;
	auto ra_g_length = test::mat_g_m * test::mat_g_n;
	auto rav_g = make_rtsav(ra_g, ra_g_length);
	auto rm_g_sz = make_matrix_size(test::mat_g_m, test::mat_g_n);

	auto rm_g = sm<
		hbrs::mpl::rtsav<double const> &, 
		matrix_size<std::size_t, std::size_t>, 
		storage_order::row_major
	>{rav_g, rm_g_sz};
	BOOST_CHECK((*size)(rm_g) == rm_g_sz);
	auto m_rm_g = (*m)(size(rm_g));
	auto n_rm_g = (*n)(size(rm_g));
	BOOST_CHECK(m_rm_g == test::mat_g_m);
	BOOST_CHECK(n_rm_g == test::mat_g_n);
	for(std::size_t i = 0; i < m_rm_g; ++i) {
		for(std::size_t j = 0; j < n_rm_g; ++j) {
			BOOST_CHECK(ra_g[i * test::mat_g_n + j] == rav_g[i * test::mat_g_n + j]);
			BOOST_CHECK(ra_g[i * test::mat_g_n + j] == rm_g[i][j]);
			BOOST_CHECK(rm_g[i][j] == (*at)(rm_g, make_matrix_index(i,j)));
			BOOST_CHECK(ra_g[i * test::mat_g_n + j] == (*at)(rm_g, make_matrix_index(i,j)));
		}
	}
	
	auto ra_h = test::mat_h;
	auto ra_h_length = test::mat_h_m * test::mat_h_n;
	auto rav_h = make_rtsav(ra_h, ra_h_length);
	auto rm_h_sz = make_matrix_size(test::mat_h_m, test::mat_h_n);
	auto rm_h = sm<
		hbrs::mpl::rtsav<double const> &, 
		matrix_size<std::size_t, std::size_t>, 
		storage_order::row_major
	>{rav_h, rm_h_sz};
	BOOST_CHECK((*size)(rm_h) == rm_h_sz);
	auto m_rm_h = (*m)(size(rm_h));
	auto n_rm_h = (*n)(size(rm_h));
	BOOST_CHECK(test::mat_h_m == m_rm_h);
	BOOST_CHECK(test::mat_h_n == n_rm_h);
	
	auto ra_i = test::mat_i;
	auto ra_i_length = test::mat_i_m * test::mat_i_n;
	auto rav_i = make_rtsav(ra_i, ra_i_length);
	auto rm_i_sz = make_matrix_size(test::mat_i_m, test::mat_i_n);
	auto rm_i = sm<
		hbrs::mpl::rtsav<double const> &, 
		matrix_size<std::size_t, std::size_t>, 
		storage_order::column_major
	>{rav_i, rm_i_sz};
	BOOST_CHECK((*size)(rm_i) == rm_i_sz);
	auto m_rm_i = (*m)(size(rm_i));
	auto n_rm_i = (*n)(size(rm_i));
	BOOST_CHECK(test::mat_i_m == m_rm_i);
	BOOST_CHECK(test::mat_i_n == n_rm_i);
	
	BOOST_CHECK(m_rm_h == m_rm_i);
	BOOST_CHECK(n_rm_h == n_rm_i);
	
	for(std::size_t i = 0; i < m_rm_h; ++i) {
		for(std::size_t j = 0; j < n_rm_h; ++j) {
			BOOST_CHECK(rm_h[i][j] == rm_i[i][j]);
		}
	}
	
	
	
	static constexpr auto crcm_g_sz = make_matrix_size(hana::size_c<test::mat_g_m>, test::mat_g_n);
	static constexpr auto crcm_g = make_sm(cav_g, crcm_g_sz, row_major_c);
	static constexpr auto m_crcm_g = (*m)(size(crcm_g));
	static constexpr auto n_crcm_g = (*n)(size(crcm_g));
	static_assert(m_crcm_g == test::mat_g_m, "");
	static_assert(n_crcm_g == test::mat_g_n, "");
	static_assert(crcm_g[hana::size_c<2>][3u] == 20, "");
	static_assert(crcm_g[2ul][3u] == 20, "");
	static_assert(make_sm(cav_g, crcm_g_sz, row_major_c)[2ul][3ul] == 20, "");
	
	auto crrm_g_sz = make_matrix_size(hana::size_c<test::mat_g_m>, test::mat_g_n);
	auto crrm_g = make_sm(rav_g, crrm_g_sz, row_major_c);
	auto m_crrm_g = (*m)(size(crrm_g));
	auto n_crrm_g = (*n)(size(crrm_g));
	BOOST_CHECK(m_crrm_g == test::mat_g_m);
	BOOST_CHECK(n_crrm_g == test::mat_g_n);
	BOOST_CHECK(crrm_g[hana::size_c<2>][3] == 20);
	BOOST_CHECK(crrm_g[2ul][3] == 20);
	BOOST_CHECK(make_sm(rav_g, crrm_g_sz, row_major_c)[2ul][3ul] == 20);
	
	static constexpr auto rccm_g_sz = make_matrix_size(test::mat_g_m, hana::size_c<test::mat_g_n>);
	static constexpr auto rccm_g = make_sm(cav_g, rccm_g_sz, row_major_c);
	static constexpr auto m_rccm_g = (*m)(size(rccm_g));
	static constexpr auto n_rccm_g = (*n)(size(rccm_g));
	static_assert(m_rccm_g == test::mat_g_m, "");
	static_assert(n_rccm_g == test::mat_g_n, "");
	static_assert(rccm_g[2ul][hana::size_c<3>] == 20, "");
	static_assert(rccm_g[2ul][3ul] == 20, "");
	static_assert(make_sm(cav_g, rccm_g_sz, row_major_c)[2ul][3ul] == 20, "");
	
	auto rcrm_g_sz = make_matrix_size(test::mat_g_m, hana::size_c<test::mat_g_n>);
	auto rcrm_g = make_sm(rav_g, rcrm_g_sz, row_major_c);
	auto m_rcrm_g = (*m)(size(rcrm_g));
	auto n_rcrm_g = (*n)(size(rcrm_g));
	BOOST_CHECK(m_rcrm_g == test::mat_g_m);
	BOOST_CHECK(n_rcrm_g == test::mat_g_n);
	BOOST_CHECK(rcrm_g[2ul][hana::size_c<3>] == 20);
	BOOST_CHECK(rcrm_g[2ul][3ul] == 20);
	BOOST_CHECK(make_sm(rav_g, rcrm_g_sz, row_major_c)[2ul][3ul] == 20);
	
	
	
	
	auto rrv_g = make_srv(rav_g);
	BOOST_CHECK(rrv_g[2*test::mat_g_n+3] == 20);
	BOOST_CHECK(rrv_g[11] == 20);
	
	int v0{0};
	std::size_t i0 = std::rand() % test::mat_g_m;
	std::size_t i1 = std::rand() % test::mat_g_n;
	#define _FAKE_USE(x)                                                                                               \
		std::cout << x; __asm__ __volatile__("" :: "m" (x));
	
	_FAKE_USE(i0);
	
	asm("nopw 0x000001");
	v0 = crrm_g[hana::size_c<1>][1];
	_FAKE_USE(v0);
	asm("nopw 0x000002");
	v0 = crrm_g[1ul][2];
	_FAKE_USE(v0);
	asm("nopw 0x000003");
	v0 = rrv_g[1*test::mat_g_n +1];
	_FAKE_USE(v0);
	asm("nopw 0x000004");
	v0 = rrv_g[4];
	_FAKE_USE(v0);
	asm("nopw 0x000005");
	v0 = crrm_g[i0][2];
	_FAKE_USE(v0);
	asm("nopw 0x000006");
	v0 = crrm_g[0ul][i1];
	_FAKE_USE(v0);
	asm("nopw 0x000007");
	v0 = rrv_g[i0];
	_FAKE_USE(v0);
	asm("nopw 0x000008");
	v0 = rcrm_g[i0][1ul];
	_FAKE_USE(v0);
	asm("nopw 0x000009");
	v0 = rrv_g[i0 * test::mat_g_m + 1ul];
	_FAKE_USE(v0);
	asm("nopw 0x000010");
	v0 = rcrm_g[i0][0ul];
	_FAKE_USE(v0);
	asm("nopw 0x000011");
	
	#undef _FAKE_USE
	
}

BOOST_AUTO_TEST_SUITE_END()
