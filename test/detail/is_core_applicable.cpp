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


#define BOOST_TEST_MODULE is_core_applicable_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/equal.hpp>
#include <hbrs/mpl/detail/is_core_applicable.hpp>

BOOST_AUTO_TEST_SUITE(is_core_applicable_test)

namespace hana = boost::hana;
struct t1 {};

struct t2 {
	static constexpr hana::int_<1337>
	apply(hana::int_<42>) {
		return {};
	}
};

struct t3 {
	static constexpr hana::int_<1337>
	apply(hana::int_<42>) = delete;
};

BOOST_AUTO_TEST_CASE(is_core_applicable_test_1) {
	using namespace hbrs::mpl;
	
	static_assert(!detail::is_core_applicable(t1{}, hana::int_c<42>), "");
	static_assert( detail::is_core_applicable(t2{}, hana::int_c<42>), "");
	static_assert(!detail::is_core_applicable(t3{}, hana::int_c<42>), "");
	static_assert(t2::apply(hana::int_c<42>) == hana::int_c<1337>, "");
}

BOOST_AUTO_TEST_SUITE_END()
