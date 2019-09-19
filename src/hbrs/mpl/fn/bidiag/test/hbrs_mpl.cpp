/* Copyright (c) 2019 Abdullah Güntepe, <abdullah@guentepe.com>
 * Copyright (c) 2019 Jakob Meng, <jakobmeng@web.de>
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


#define BOOST_TEST_MODULE fn_bidiag_hbrs_mpl_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/fn/bidiag.hpp>
#include <hbrs/mpl/fn/multiply.hpp>
#include <hbrs/mpl/fn/transpose.hpp>
#include <hbrs/mpl/dt/storage_order.hpp>
#include <hbrs/mpl/dt/rtsam.hpp>

#include <hbrs/mpl/detail/test.hpp>

namespace utf = boost::unit_test;
namespace tt = boost::test_tools;

#define _TOL 0.000000001

BOOST_AUTO_TEST_SUITE(fn_bidiag_hbrs_mpl_test)

using hbrs::mpl::detail::environment_fixture;
BOOST_TEST_GLOBAL_FIXTURE(environment_fixture);

//TODO: Refactor like fn_variance_hbrs_mpl_test
BOOST_AUTO_TEST_CASE(rtsam_, * utf::tolerance(_TOL)) {
	using namespace hbrs::mpl;
	using hbrs::mpl::decompose_mode;
	
	rtsam<double, storage_order::row_major> A {
		{ 16,  2,  3, 13,
		   5, 11, 10,  8,
		   9,  7,  6, 12,
		   4, 14, 15,  1},
		make_matrix_size(4, 4)
	};
	
	rtsam<double, storage_order::row_major> A2 {
		{ 2,  2,  3,
		  9,  8,  1,
		 15,100,  7,
		 99,  1,  2,
		  5,  7,  3},
		make_matrix_size(5,3)
	};

	auto B  = bidiag(A , make_bidiag_control(decompose_mode::complete));
	auto B2 = bidiag(A , make_bidiag_control(decompose_mode::complete));
	auto B3 = bidiag(A2, make_bidiag_control(decompose_mode::complete));

	auto C  =  B.u() *  B.b() * transpose( B.v());
	auto C2 = B2.u() * B2.b() * transpose(B2.v());
	auto C3 = B3.u() * B3.b() * transpose(B3.v());

	HBRS_MPL_TEST_MMEQ(C, A, false);
	HBRS_MPL_TEST_MMEQ(C2, A, false);
	HBRS_MPL_TEST_MMEQ(C3, A2, false);
}

BOOST_AUTO_TEST_SUITE_END()
