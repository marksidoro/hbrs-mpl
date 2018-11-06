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


#define BOOST_TEST_MODULE lambda_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

//TODO: Rework to not include any fused impls!

#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/plus.hpp>
#include <hbrs/mpl/fn/minus.hpp>
#include <hbrs/mpl/fn/if.hpp>

#include <boost/hana/integral_constant.hpp>
#include <tuple>

struct lambda1 {
	template<typename T>
	constexpr decltype(auto)
	operator()(T && t) const {
		using hbrs::mpl::at;
		return at(std::tuple<int>{1337}, HBRS_MPL_FWD(t));
	}
};

struct lambda2 {
	template<typename U, typename V>
	constexpr decltype(auto)
	operator()(U && u, V && v) const {
		using hbrs::mpl::at;
		return at(HBRS_MPL_FWD(v), HBRS_MPL_FWD(u));
	}
};

struct lambda3 {
	template<typename U, typename V>
	constexpr decltype(auto)
	operator()(U && u, V && v) const {
		using hbrs::mpl::at;
		return at(
			at(
				std::make_tuple(4711, 42, std::make_tuple(1337)),
				HBRS_MPL_FWD(u)
			), 
			HBRS_MPL_FWD(v)
		);
	}
};

BOOST_AUTO_TEST_SUITE(lambda_test)

BOOST_AUTO_TEST_CASE(lambda_test_1) {
	using namespace hbrs::mpl;
	namespace hana = boost::hana;
	using namespace hana::literals;
	
	static_assert(evaluate(lambda1{}(0_c)) == 1337, "");
	BOOST_CHECK(evaluate([](auto && v){ return at(std::tuple<int>{1337}, HBRS_MPL_FWD(v)); }(0_c)) == 1337);
	
	
	static_assert(evaluate(lambda2{}(0_c, std::tuple<int>{1337})) == 1337, "");
	BOOST_CHECK(evaluate([](auto && u, auto && v){ return at(HBRS_MPL_FWD(v), HBRS_MPL_FWD(u)); }(0_c, std::tuple<int>{1337})) == 1337);
	
	static_assert(evaluate(lambda3{}(2_c, 0_c)) == 1337, "");
	BOOST_CHECK(evaluate(
		[](auto && u, auto && v){ 
			return at(
				at(
					std::make_tuple(4711, 42, std::make_tuple(1337)),
					HBRS_MPL_FWD(u)
				), 
				HBRS_MPL_FWD(v)
			);
		}(2_c, 0_c)) == 1337);
	
// 	static_assert(
// 		at(
// 			at(
// 				std::make_tuple(4711, 42, std::make_tuple(1337)), 
// 				_0
// 			), 
// 			_1
// 		)(2_c)(0_c) == 1337, "");
// 	
// 	typedef std::tuple<int, int, std::tuple<int>> tuple_t;
// 	static constexpr tuple_t t0 = std::make_tuple(4711, 42, std::make_tuple(1337));
// 	
// 	typedef const std::tuple<hbrs::mpl::tuple_detail::at_impl_default_entry, hbrs::mpl::tuple_detail::at_impl_value_entry, hbrs::mpl::tuple_detail::at_impl_hmg_int_entry, hbrs::mpl::at_detail::impl_entry> at_impls_t;
// 	
// 	static_assert(std::is_same<at_impls_t, decltype(at_detail::at_impls_c)>::value, "");
// 	
// 
// 	
// 	typedef lambda<expression<at_t const&, tuple_t const&, lambda_variable<0> const&>, at_impls_t> lambda_t;
// 	static constexpr lambda_t t1 = at(t0, _0);
// 	
// 	typedef lambda<expression<at_t const&, tuple_t const&, lambda_variable<1> const&>, at_impls_t > lambda_t2;
// 	typedef lambda<expression<at_t const&, tuple_t const&, lambda_variable<0> const&>, at_impls_t > lambda_t2_lvdecr;
// 	
// //	typedef lambda<expression<at_t const, tuple_t const&, lambda_variable<1> const&>, at_impls_t > lambda_t2;
// //	typedef lambda<expression<at_t const, tuple_t const&, lambda_variable<0> const>, at_impls_t > lambda_t2_lvdecr;
// 	
// 	static constexpr lambda_t2 t2 = t1(_1);
// 	
// 	typedef lambda<expression<at_t const&, lambda_t2 const&, lambda_variable<0> const&>, at_impls_t > lambda_t3;
// 	static constexpr lambda_t3 t3 = at(t2, _0);
// 
//	typedef lambda<expression<at_t const, lambda_t2_lvdecr, boost::hana::integral_constant<long long, 0> &>, at_impls_t> lambda_t4;
// 	
// 	typedef lambda<expression<at_t const&, lambda_t2_lvdecr const&, boost::hana::integral_constant<long long, 0> &>, at_impls_t> lambda_t4;
// 	//typedef lambda<expression<at_t const&, lambda_t2_lvdecr, boost::hana::integral_constant<long long, 0> &>, at_impls_t> lambda_t4;
// 	static constexpr lambda_t4 t4 = t3(0_c);
// 	//static_assert(t4(2_c) == 1337, "");
// 







// 	static constexpr auto tt1 = std::make_tuple(4711, 42, std::make_tuple(1337));
// 	static constexpr auto tt2 = make_lambda(_0)(at(tt1, _0))(_1);
// 	
// 	static constexpr auto tt3 = 0_c;
// 	static constexpr auto tt4 = make_lambda(_0, _1)(at(
// 		tt2,
// 		_0
// 	))(tt3);
// 	
// 	static_assert(
// 		make_lambda(_0, _1)(at(
// 			make_lambda(_0)(at(
// 				std::make_tuple(4711, 42, std::make_tuple(1337)), 
// 				_0
// 			))(_1),
// 			_0
// 		))(0_c)(2_c) == 1337, "");
// 	
// 	static_assert(make_lambda(_1, _2).variables() == std::make_tuple( hana::type_c<index_lambda_variable<1>>, hana::type_c<index_lambda_variable<2>> ), "");
// 	
// 	static_assert(make_lambda(_1, _2)(at(std::make_tuple(1, 2, 3), 1_c))() == 2, "");
// 	
// 	static_assert(make_lambda(_1, _2)(at(_1, _2))(std::make_tuple(1, 2, 3), 1_c) == 2, "");
// 	static_assert(make_lambda(_1, _2)(at(_1, _2))(std::make_tuple(1, 2, 3))(1_c) == 2, "");
// 	
// 	make_lambda();
// 	
// 	static_assert(make_lambda(_1, _2, _3)(
// 		at(at(_1, _2), _3)
// 	)(
// 		std::make_tuple(1, std::make_tuple(2, 3)), 1_c, 0_c
// 	) == 2, "");
// 	
// 	
// 	static constexpr auto t1 = std::make_tuple(4711, 42, std::make_tuple(1337));
// 	
// 	
// 	static constexpr auto t2 = at(
// 			t1,
// 			_1
// 		);
// 	
// 	static constexpr auto t3 = make_lambda(_1)(t2);
// 	static constexpr auto t4 = t3(_3);
// 	static constexpr auto t5 = make_lambda(_3)(t4);
// 	static constexpr auto t6 = t5(0_c);
// 	
// 	static_assert(t6 == 4711, "");
// 	
// 	static constexpr auto t8 = make_lambda(_1)(at(t1,_1))(0_c);
// 	static_assert(t8 == 4711, "");
// 	
// 	static constexpr auto t9 = make_lambda(_1)(at(t1,_1))(_3);
// 	static constexpr auto t10 = make_lambda(_3)(make_lambda(_1)(at(t1,_1))(_3));
// 	
// 	static constexpr auto t11 = t10(0_c);
// 	static_assert(t11 == 4711, "");
// 	
// 	static constexpr auto t12 = make_lambda(_3)(make_lambda(_1)(at(t1,_1))(_3))(0_c);
// 	static_assert(t12 == 4711, "");
// 	
// 	static_assert(
// 		make_lambda(_1, _2)(
// 			make_lambda(_1, _2)(at(_1,_2))(_2, _1)
// 		)(0_c, std::make_tuple(1337))
// 		== 1337, "");
// 	
// 	static_assert(
// 		make_lambda(_2, _3)(
// 			at(
// 				make_lambda(_1)(
// 					at(
// 						std::make_tuple(4711, 42, std::make_tuple(1337)), 
// 						_1
// 					)
// 				)(_3),
// 				_2
// 			)
// 		)
// 		(0_c)(2_c) == 1337, "");
// 	
// 	struct tag1{};
// 	struct tag2{};
// 	static constexpr lambda_variable<tag1> lv1{};
// 	static constexpr lambda_variable<tag2> lv2{};
// 	
// 	static_assert(
// 		make_lambda(lv1, lv2)
// 		(
// 			at(lv1, lv2)
// 		)
// 		(
// 			std::make_tuple(4711, 42, std::make_tuple(1337))
// 		)
// 		(
// 			0_c
// 		)
// 		== 4711, "");
// 	
// 	
// 	static constexpr auto el1 = make_lambda()(0_c);
// 	static_assert(el1() == 0_c, "");
// 	
// 	static_assert(make_lambda(_0)(0_c)(1_c) == 0_c, "");
// 	
// 	
// 	
// 	/* variable shadowing test */
// 	static_assert(
// 		make_lambda(_0, _1)(
// 			make_lambda(_0, _1)(
// 				minus(_0, _1)
// 			)(_1, _0)
// 		)(2_c, 3_c) == 1_c
// 	, "");
// 	
// 	static_assert(make_lambda(_2)(_2)(1337_c) == 1337_c, "");
// 	
// 	
// 	static constexpr auto nested1 = make_lambda(_0)(
// 		if_(
// 			not_equal(_0, 1337_c),
// 			make_lambda(_1)(42_c),
// 			make_lambda(_2)(_2)
// 		)(_0)
// 	);
// 	
// 	static_assert(nested1(1_c) == 42_c, "");
// 	static_assert(nested1(1337_c) == 1337_c, "");
// 	
// 	HBRS_MPL_MAKE_LAMBDA_VARIABLE(x);
// 	HBRS_MPL_MAKE_LAMBDA_VARIABLE(y);
// 	
// 	constexpr auto l1 = make_lambda(x, y)(plus(x,y));
// 	static_assert(l1(1,2) == 3, "");
// 	BOOST_CHECK(l1(1,2) == 3);
// 	
// 	constexpr auto l2 = make_lambda(x, y)(x+y);
// 	static_assert(l2(1,2) == 3, "");
// 	BOOST_CHECK(l2(1,2) == 3);
}

BOOST_AUTO_TEST_SUITE_END()
