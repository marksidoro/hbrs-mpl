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

#define BOOST_TEST_MODULE expand_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/fn/expand.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/dt/rtsam.hpp>
#include <hbrs/mpl/dt/srv.hpp>
#include <vector>

#include "../detail.hpp"

BOOST_AUTO_TEST_SUITE(expand_test)

BOOST_AUTO_TEST_CASE(srv_expand) {
	using namespace hbrs::mpl;
	
	auto const a = rtsam<double, storage_order::row_major>{
		{
			1.,2.,3.,
			1.,2.,3.,
			1.,2.,3.,
		},
		matrix_size<std::size_t, std::size_t>{3u,3u}
	};
	
	srv<std::vector<double>> rv{ std::vector<double>{ 1., 2., 3. } };
	
	rtsam<double, storage_order::row_major> b = (*expand)(rv, (*size)(a));
	_BOOST_TEST_MMEQ(a, b, false);
}

BOOST_AUTO_TEST_SUITE_END()
