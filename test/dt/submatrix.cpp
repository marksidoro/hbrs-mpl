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

#define BOOST_TEST_MODULE submatrix_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/dt/submatrix.hpp>
#include <hbrs/mpl/dt/ctsam.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>

#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <boost/hana/integral_constant.hpp>
#include <array>

#include "../detail.hpp"

BOOST_AUTO_TEST_SUITE(submatrix_test)

BOOST_AUTO_TEST_CASE(m_n_size_assignment) {
	using namespace hbrs::mpl;
	
	static constexpr auto a = make_ctsam(
		std::array<double, 9>{{
			1., 2., 3.,
			4., 5., 6.,
			7., 8., 9.
		}},
		make_matrix_size(hana::size_c<3>, hana::size_c<3>),
		row_major_c
	);
	
	auto const b = make_ctsam(
		std::array<double, 6>{{
			1.,2.,3.,
			4.,5.,6.,
		}},
		make_matrix_size(hana::size_c<2>, hana::size_c<3>),
		row_major_c
	);
	
	auto const c = make_ctsam(
		std::array<double, 6>{{
			4.,5.,6.,
			7.,8.,9.
		}},
		make_matrix_size(hana::size_c<2>, hana::size_c<3>),
		row_major_c
	);
	
	auto const d = make_ctsam(
		std::array<double, 6>{{
			1.,2.,
			4.,5.,
			7.,8.
		}},
		make_matrix_size(hana::size_c<3>, hana::size_c<2>),
		row_major_c
	);
	
	auto const e = make_ctsam(
		std::array<double, 6>{{
			2.,3.,
			5.,6.,
			8.,9.
		}},
		make_matrix_size(hana::size_c<3>, hana::size_c<2>),
		row_major_c
	);
	
	static constexpr auto rb0 = make_submatrix(a, make_matrix_index(0u,0u), make_matrix_size(2u,3u));
	static constexpr auto rc0 = make_submatrix(a, make_matrix_index(1u,0u), make_matrix_size(2u,3u));
	static constexpr auto rd0 = make_submatrix(a, make_matrix_index(0u,0u), make_matrix_size(3u,2u));
	static constexpr auto re0 = make_submatrix(a, make_matrix_index(0u,1u), make_matrix_size(3u,2u));
	
	static_assert( (*at)(rb0, make_matrix_index(0u,0u)) == (*at)(a, make_matrix_index(0u,0u)), "");
	static_assert( rb0[0u][0u] == (*at)(a, make_matrix_index(0u,0u)), "");
	
	_BOOST_TEST_MMEQ(b, rb0, false);
	_BOOST_TEST_MMEQ(c, rc0, false);
	_BOOST_TEST_MMEQ(d, rd0, false);
	_BOOST_TEST_MMEQ(e, re0, false);
}

BOOST_AUTO_TEST_SUITE_END()
