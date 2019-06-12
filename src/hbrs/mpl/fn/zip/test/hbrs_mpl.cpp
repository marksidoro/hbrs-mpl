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


#define BOOST_TEST_MODULE zip_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/fn/zip.hpp>
#include <hbrs/mpl/fn/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/equal.hpp>
#include <boost/range/irange.hpp>
#include <tuple>
#include <vector>
#include <array>

BOOST_AUTO_TEST_SUITE(zip_test)

BOOST_AUTO_TEST_CASE(array_vector_irange_tuple) {
	using namespace hbrs::mpl;
	using namespace hana::literals;
	
	static constexpr auto const a1 = std::array<int, 3>{1,2,3};
	static constexpr auto const a2 = std::array<int, 3>{4,5,6};
	static auto const v1 = std::vector<int>{1,2,3};
	static auto const v2 = std::vector<int>{4,5,6};
	static auto const r1 = boost::irange(1,4);
	static auto const r2 = boost::irange(4,7);
	static constexpr auto const t1 = std::make_tuple(1_c, 2_c, 3_c);
	static constexpr auto const t2 = std::make_tuple(4_c, 5_c, 6_c);
	
	static constexpr zas<std::array<int, 3>, std::array<int, 3>> z1 = (*zip)(a1, a2);
	static zas<std::vector<int>, std::vector<int>> z2 = (*zip)(v1, v2);
	static zas<boost::integer_range<int>, boost::integer_range<int>> z3 = (*zip)(r1, r2);
	static constexpr zas<
		std::tuple<hana::llong<1>,hana::llong<2>,hana::llong<3>>,
		std::tuple<hana::llong<4>,hana::llong<5>,hana::llong<6>>
	> z4 = (*zip)(t1, t2);
	
	std::vector<std::tuple<double, int>> z5 = detail::zip_impl_std_tuple_vector{}(
		std::vector<double>{3.,4.,5.}, std::vector<int>{7,8,9}
	);
	BOOST_TEST((z5 == std::vector<std::tuple<double, int>>{ {3.,7}, {4.,8}, {5., 9} }));
	
	//TODO: Compare contents of zipped sequences!
}

BOOST_AUTO_TEST_SUITE_END()
