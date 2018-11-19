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


#define BOOST_TEST_MODULE columns_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/fn/columns.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/dt/rtsam.hpp>
#include <boost/hana/integral_constant.hpp>
#include <vector>

BOOST_AUTO_TEST_SUITE(columns_test)

BOOST_AUTO_TEST_CASE(rtsam_columns) {
	using namespace hbrs::mpl;
	
	auto const a = rtsam<double, storage_order::row_major>{
		{
			1.,2.,3.,
			4.,5.,6.
		},
		matrix_size<std::size_t, std::size_t>{2u,3u}
	};
	
	auto const a_sz = (*size)(a);
	auto const a_m = (*m)(a_sz);
	auto const a_n = (*n)(a_sz);
	
	auto b = (*columns)(a);
	auto const b_sz = (*size)(b);
	BOOST_TEST(b_sz == a_n);
	
	for(std::size_t j = 0; j < a_n; ++j) {
		auto column = (*at)(b, j);
		BOOST_TEST((*size)(column) == a_m);
		for(std::size_t i = 0; i < a_m; ++i) {
			BOOST_TEST((*at)(column, i) == (*at)(a, make_matrix_index(i,j)));
		}
	}
}

BOOST_AUTO_TEST_SUITE_END()
