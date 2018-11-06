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


#define BOOST_TEST_MODULE fold_left_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

//#include <hbrs/mpl/core/lambda.hpp>
//#include <hbrs/mpl/fn/if.hpp>

#include <hbrs/mpl/fn/fold_left.hpp>
#include <hbrs/mpl/fn/plus.hpp>
#include <hbrs/mpl/fn/equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>
#include <tuple>
#include <vector>

BOOST_AUTO_TEST_SUITE(fold_left_test)

BOOST_AUTO_TEST_CASE(fold_left_test_1) {
	using namespace hbrs::mpl;
	using namespace hana::literals;
	//TODO: Uncomment all lambda stuff!
// 	HBRS_MPL_MAKE_LAMBDA_VARIABLE(the_tuple);
// 	HBRS_MPL_MAKE_LAMBDA_VARIABLE(the_tuple2);
// 	
// 	static constexpr auto is_empty = make_lambda(the_tuple)(
// 		equal(length(the_tuple), hana::size_c<0>)
// 	);
// 	
// 	static_assert(is_empty(std::make_tuple()), "");
// 	static_assert(!is_empty(std::make_tuple(1)), "");
// 	
// 	static constexpr auto is_empty2 = make_lambda(the_tuple2)(is_empty(the_tuple2));
// 	static_assert(is_empty2(std::make_tuple()), "");
// 	static_assert(!is_empty2(std::make_tuple(1)), "");
// 	
// 	static constexpr auto is_empty3 = make_lambda(the_tuple)(is_empty(the_tuple));
// 	static_assert(is_empty3(std::make_tuple()), "");
// 	static_assert(!is_empty3(std::make_tuple(1)), "");
	
	
	BOOST_CHECK(detail::fold_left_impl_std_vector{}(std::vector<int>{1,2,3}, 0, plus) == 6);
	BOOST_CHECK(  (*fold_left)(std::array<int, 3>{{1,2,3}},        0,   plus) == 6  );
	
	static_assert(detail::fold_left_impl_std_array{}(std::array<int, 3>{{1,2,3}},        0,   plus) == 6, "");
	static_assert((*fold_left)(std::array<int, 3>{{1,2,3}},        0,   plus) == 6, "");
	static_assert((*fold_left)(hana::make_tuple(1_c, 2_c, 3_c), 0_c, plus) == 6_c, "");
	static_assert((*fold_left)(hana::make_tuple(1, 2, 3), 0, plus) == 6, "");
	static_assert((*fold_left)(std::make_tuple(1_c, 2_c, 3_c), 0_c, plus) == 6_c, "");
	BOOST_CHECK(  (*fold_left)(std::vector<int>{1,2,3},        0,   plus) == 6  );
	
	struct plus_{ int operator()(int a, int b) { return a+b; } };
	BOOST_CHECK(  (*fold_left)(std::vector<int>{1,2,3},        0,   plus_{}) == 6  );
	
	
	
	
// 	static constexpr auto sum_fold = make_lambda(_0)(fold_left(_0, 0_c, plus));
// 	static_assert(sum_fold(std::make_tuple(1_c,2_c)) == 3_c, "");
// 	
// 	static constexpr auto zero = make_lambda(_0)(0_c);
// 	static_assert(zero() == 0_c, "");
// 	
// 	static constexpr auto sum_fs = make_lambda(the_tuple)(
// 		if_(is_empty(the_tuple), zero, sum_fold)(the_tuple)
// 	);
// 	
// 	static_assert(sum_fs(std::make_tuple()) == 0_c, "");
// 	static_assert(sum_fs(std::make_tuple(1_c, 2_c)) == 3_c, "");
// 	
// 	static constexpr auto sum = make_lambda(the_tuple)(
// 		if_(
// 			is_empty(the_tuple),
// 			make_lambda(_1)(0_c),
// 			make_lambda(_0)(fold_left(_0, 0_c, plus))
// 		)(the_tuple)
// 	);
// 	
// 	static_assert(sum(std::make_tuple()) == 0_c, "");
// 	static_assert(sum(std::make_tuple(1_c, 2_c, 3_c)) == 6_c, "");
// 	
// 	static_assert(sum(hana::make_tuple(1_c, 2_c, 3_c)) == 6_c, "");

}

BOOST_AUTO_TEST_SUITE_END()
