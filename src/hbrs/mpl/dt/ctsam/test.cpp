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

#define BOOST_TEST_MODULE ctsam_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/dt/ctsam.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <boost/hana/integral_constant.hpp>
#include <array>

BOOST_AUTO_TEST_SUITE(ctsam_test)

BOOST_AUTO_TEST_CASE(m_n_size_assignment) {
	using namespace hbrs::mpl;
	
	static constexpr auto m1_sz = make_matrix_size(hana::size_c<2>, hana::size_c<3>);
	static constexpr auto m1 = make_ctsam(
		std::array<double, 6>{{
			1., 2., 3.,
			4., 5., 6.
		}},
		m1_sz, 
		row_major_c
	);
	
	for(std::size_t i = 0; i < m1_sz.m(); ++i) {
		for(std::size_t j = 0; j < m1_sz.n(); ++j) {
			BOOST_TEST((*at)(m1, make_matrix_index(i,j)) == (double)(i*m1_sz.n() + j+1));
		}
	}
}

BOOST_AUTO_TEST_SUITE_END()
