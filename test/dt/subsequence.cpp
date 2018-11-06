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

#define BOOST_TEST_MODULE subsequence_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/dt/subsequence.hpp>
#include <array>

#include <hbrs/mpl/fn/equal.hpp>
#include "../detail.hpp"

BOOST_AUTO_TEST_SUITE(subsequence_test)

BOOST_AUTO_TEST_CASE(m_n_size_assignment) {
	using namespace hbrs::mpl;
	
	static constexpr auto a = std::array<double, 9>{{1., 2., 3., 4., 5., 6., 7., 8., 9.}};
	static constexpr auto b = std::array<double, 6>{{1., 2., 3., 4., 5., 6.            }};
	static constexpr auto c = std::array<double, 6>{{            4., 5., 6., 7., 8., 9.}};
	static constexpr auto d = std::array<double, 3>{{            4., 5., 6.            }};
	
	static constexpr auto rb = make_subsequence(a, 0u, 6u);
	static constexpr auto rc = make_subsequence(a, 3u, 6u);
	static constexpr auto rd = make_subsequence(a, 3u, 3u);
	
	_BOOST_TEST_VVEQ(b, rb, false);
	_BOOST_TEST_VVEQ(c, rc, false);
	_BOOST_TEST_VVEQ(d, rd, false);
}

BOOST_AUTO_TEST_SUITE_END()
