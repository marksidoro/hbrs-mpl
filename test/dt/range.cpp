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

#define BOOST_TEST_MODULE range_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/dt/range.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/equal.hpp>
#include <hbrs/mpl/fn/first.hpp>
#include <hbrs/mpl/fn/last.hpp>

BOOST_AUTO_TEST_SUITE(range_test)

BOOST_AUTO_TEST_CASE(range_first_last) {
	using namespace hbrs::mpl;
	static constexpr range<hana::int_<0>, hana::int_<1>> rng = make_range(hana::int_c<0>, hana::int_c<1>);
	
	detail::first_impl_range{}(rng);
	static constexpr hana::int_<0> first_ = (*first)(rng);
	detail::last_impl_range{}(rng);
	static constexpr hana::int_<1> last_ = (*last)(rng);
}

BOOST_AUTO_TEST_SUITE_END()
