/* Copyright (c) 2018 Jakob Meng, <jakobmeng@web.de>
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

#define BOOST_TEST_MODULE mean_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/fn/mean.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/columns.hpp>
#include <hbrs/mpl/fn/equal.hpp>

#include <hbrs/mpl/dt/srv.hpp>
#include <hbrs/mpl/dt/sm.hpp>
#include <hbrs/mpl/dt/ctsav.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>
#include <hbrs/mpl/dt/storage_order.hpp>
#include <hbrs/mpl/dt/rtsam.hpp>

#include <hbrs/mpl/detail/test.hpp>

#include <hbrs/mpl/detail/test.hpp>

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>
#include <tuple>
#include <vector>

#include "../data.hpp"

BOOST_AUTO_TEST_SUITE(mean_test)

BOOST_AUTO_TEST_CASE(column_mean_sm_ctsav_icsz) {
	using namespace hbrs::mpl;
	
	auto const a = make_sm(
		make_ctsav(test::mat_j), make_matrix_size(hana::size_c<test::mat_j_m>, hana::size_c<test::mat_j_n>), row_major_c
	);
	
	auto a_m = (*m)(size(a));
	auto a_n = (*n)(size(a));
	
	detail::mean_impl_smcs_sm_ctsav_icsz{}((*columns)(a));
	auto cm = (*mean)(columns(a));
	auto cm_sz = (*size)(cm);
	
	BOOST_TEST((*equal)(cm_sz, a_n));
	
	for(std::size_t j = 0; j < a_n; ++j) {
		double cm_ = 0.;
		for(std::size_t i = 0; i < a_m; ++i) {
			cm_ += (*at)(a, make_matrix_index(i,j));
		}
		
		cm_ /= a_m;
		BOOST_TEST((*at)(cm, j) == cm_);
	}
	
	srv<std::vector<double>> ocm{ std::vector<double>{
		24.75, 24.25, 23.75, 24.25, 24.50, 24.25, 24.25, 24.50, 24.00, 24.50, 24.50, 24.50, 24.50
	} };
	BOOST_TEST((*equal)(size(ocm), a_n));
	HBRS_MPL_TEST_VVEQ(cm, ocm, false);
}

BOOST_AUTO_TEST_CASE(column_mean_rtsam) {
	using namespace hbrs::mpl;
	
	auto const a = make_rtsam(
		std::vector<double>(test::mat_j+0, test::mat_j+test::mat_j_m*test::mat_j_n),
		make_matrix_size(test::mat_j_m, test::mat_j_n), 
		row_major_c
	);
	
	auto a_m = (*m)(size(a));
	auto a_n = (*n)(size(a));
	
	detail::mean_impl_smcs_rtsam{}((*columns)(a));
	auto cm = (*mean)(columns(a));
	auto cm_sz = (*size)(cm);
	
	BOOST_TEST((*equal)(cm_sz, a_n));
	
	for(std::size_t j = 0; j < a_n; ++j) {
		double cm_ = 0.;
		for(std::size_t i = 0; i < a_m; ++i) {
			cm_ += (*at)(a, make_matrix_index(i,j));
		}
		
		cm_ /= a_m;
		BOOST_TEST((*at)(cm, j) == cm_);
	}
	
	srv<std::vector<double>> ocm{ std::vector<double>{
		24.75, 24.25, 23.75, 24.25, 24.50, 24.25, 24.25, 24.50, 24.00, 24.50, 24.50, 24.50, 24.50
	} };
	BOOST_TEST((*equal)(size(ocm), a_n));
	HBRS_MPL_TEST_VVEQ(cm, ocm, false);
}

BOOST_AUTO_TEST_SUITE_END()
