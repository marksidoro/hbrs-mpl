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


#define BOOST_TEST_MODULE at_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/dt/ctsav.hpp>
#include <hbrs/mpl/dt/rtsav.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/range/irange.hpp>
#include <tuple>
#include <type_traits>
#include <cstdint>
#include <array>
#include <tuple>

#define ADDITIONAL_TESTS

BOOST_AUTO_TEST_SUITE(at_test)

BOOST_AUTO_TEST_CASE(at_array) {
	using namespace hbrs::mpl;
	
	static constexpr auto a = std::array<int, 3>{{1,2,3}};
	static_assert(a[0] == 1, "");
	static_assert((*at)(a, 0) == 1, "");
}

BOOST_AUTO_TEST_CASE(at_test_1) {
	using namespace hbrs::mpl;
	using namespace boost::hana::literals;
	
	constexpr static int a[] = { 1, 2, 3 };
	constexpr auto const a_v = make_ctsav(a);
	
	int b[] = { 1, 2, 3 };
	auto b_v = make_ctsav(b);
	
#if defined(ADDITIONAL_TESTS)
// 	static_assert(
// 		hana::is_valid(detail::has_at_op_test{}, type_c<decltype(a)>, type_c<std::size_t>)
// 		,"");
// 	
// 	static_assert(
// 		detail::has_at_op(type_c< decltype(a) >, type_c< std::size_t >)
// 		, "");
// 	
// 	static_assert(
// 		detail::has_at_op(type_c< ctsav<const int, 3> >, type_c< std::size_t >)
// 		, ""
// 	);
// 	
// 	static_assert(
// 		!detail::has_at_method(type_c< ctsav<const int, 3> >, type_c< std::size_t >)
// 		, ""
// 	);
// 	
// 	detail::at_impl_op{}(ctsav<const int, 3>{a}, std::size_t{1});
// 	
// 	detail::at_impl_op{}(ctsav<const int, 3>{a}, 1_c);
// 	
// 	detail::at_impl_op{}(a_v, 1_c);
// 	
// 	static_assert(
// 		detail::at_impl_op{}(a_v, 1_c) == 2
// 		, "");
	
	//TODO: Replace with is_invokable
// 	static_assert(
// 		eager_evaluator_detail::has_apply( type_c< detail::impl_entry >, type_c< ctsav<const int, 3> >, type_c< std::size_t> )
// 		, "");

	
	//TODO: Replace with is_invokable_filter from is_invokable_detail
// 	static_assert(
// 		eager_evaluator_detail::is_invokable_filter< 
// 			expression<at_t &&, ctsav<const int, 3> &&, std::size_t &&>
// 		>{}(
// 			detail::impl_entry{}
// 		)
// 		, "");
	
	//TODO: Replace with is_invokable_filter from is_invokable_detail
// 	static_assert(
// 		eager_evaluator_detail::is_invokable_filter< 
// 			expression<const at_t, ctsav<const int, 3> const&, boost::hana::integral_constant<long long, 1> >
// 		>{}(
// 			detail::impl_entry{}
// 		)
// 		, "");
	
	//typedef decltype( make_expression(at, a_v, 1_c) ) expression_type;
	
	//TODO: Replace with is_invokable_filter from is_invokable_detail
// 	static_assert(
// 		boost::hana::length(
// 			boost::hana::filter(
// 				//TODO: Why does filter() require a copy (const& failes!)?
// 				//boost::hana::tuple<detail::impl_entry>{
// 				//static_cast<boost::hana::tuple<detail::impl_entry> const&> (
// 					boost::hana::make_tuple(detail::impl_entry{})
// 				//)
// 				//}
// 				, 
// 				
// 				eager_evaluator_detail::is_invokable_filter<
// 					expression_type
// 					//expression<const at_t, ctsav<const int, 3> const&, boost::hana::integral_constant<long long, 1> >
// 				>{}
// 			)
// 		) > boost::hana::size_c<0>, "");

	
// 	static_assert(
// 		std::is_same<
// 			decltype( make_expression(at, a_v, 1_c) ),
// 			expression<at_t const&, ctsav<const int, 3> const&, boost::hana::integral_constant<long long, 1> >
// 		>::value, "");
	
	//TODO: Replace with find_invokables
// 	static_assert(
// 		boost::hana::length(
// 			eager_evaluator_detail::find_invokables<
// 				expression<at_t const&, ctsav<const int, 3> const&, boost::hana::integral_constant<long long, 1> >, 
// 				detail::impl_entry
// 			>(
// 				make_expression(at, a_v, 1_c),
// 				std::make_tuple(detail::impl_entry{})
// 			)
// 		) > boost::hana::size_c<0>, "");
	
	

	//TODO: Replace with find_invokables
// 	static_assert(
// 		boost::hana::length(
// 			eager_evaluator_detail::find_invokables(
// 				make_expression(at, a_v, 1_c),
// 				std::make_tuple(detail::impl_entry{})
// 			)
// 		) > boost::hana::size_c<0>, "");

#endif
	
	static_assert((*at)(a_v, 1_c) == 2, "");
	
	static_assert((*at)(a_v, 1) == 2, "");
	BOOST_CHECK((*at)(b_v, 0) == 1);
	
	int c[] = { 1, 2, 3 };
	int* cp = c;
	auto c_v = make_rtsav(cp, sizeof(c)/sizeof(c[0]));
	BOOST_CHECK((*at)(c_v, 1) == 2);
	
	static_assert(
		(*at)(
			at(
				hana::make_tuple(hana::make_tuple(hana::size_c<1337>)), 
				hana::size_c<0>
			), 
			hana::size_c<0>
		) == hana::size_c<1337>, "");
	
	static_assert(detail::at_impl_std_tuple_hmg_integral{}(std::make_tuple(1, 2, 3), 1) == 2, "");
	static_assert((*at)(std::make_tuple(1, 2, 3), 1) == 2, "");
	
	//static_assert(boost::hana::length(HBRS_MPL_AT_IMPLS) == boost::hana::size_c<4>, "");
	
#if defined(ADDITIONAL_TESTS)
	//static_assert(detail::at_impl_std_tuple_default{}(std::tuple<int>{1337}, std::integral_constant<int, 0>{}) == 1337, "");
	static_assert(detail::at_impl_std_tuple{}(std::tuple<int>{1337}, std::integral_constant<int, 0>{}) == 1337, "");
#endif
	
// 	static_assert(
// 		eager_evaluator_detail::invoke(
// 			tuple_detail::at_impl_default_entry{}, 
// 			std::make_tuple(std::tuple<int>{1337}, std::integral_constant<int, 0>{}), std::make_index_sequence<2>{}
// 		) == 1337, "");
	
	static_assert((*at)(std::tuple<int>{1337}, std::integral_constant<int, 0>{}) == 1337, "");
	
	static_assert((*at)(std::tuple<int>{1337}, 0_c) == 1337, "");
	
// 	static constexpr auto zero_hc = 0_c;
// 	static constexpr auto zero_ic = std::integral_constant<int, 0>{};
// 	static_assert(tuple_detail::at_impl_default{}(std::tuple<int>{1337}, zero_hc) == 1337, "");
// 	static_assert(tuple_detail::at_impl_default{}(std::tuple<int>{1337}, zero_ic) == 1337, "");
// 	
// 	static_assert(tuple_detail::at_impl_default{}(std::tuple<int>{1337}, 0_c) == 1337, "");
// 	static_assert(tuple_detail::at_impl_default{}(std::tuple<int>{1337}, std::integral_constant<int, 0>{}) == 1337, "");
	
	using namespace detail;
// 	static_assert(
// 		evaluate(
// 			static_cast< expression<at_t const&, std::tuple<int> const&, boost::hana::llong<0> const&> >(
// 				make_expression(
// 					static_cast<at_t const&>(at),
// 					static_cast<std::tuple<int> const&>(std::tuple<int>{1337}), 
// 					static_cast<boost::hana::llong<0> const&>(0_c)
// 				)
// 			),
// 			HBRS_MPL_FUSE_AT_IMPLS
// 		) == 1337, "");
	
	static_assert(detail::apply_at(hana::id, std::tuple<int, int>{1, 2}, 0) == 1, "");
	
	
	std::tuple<int, int> t_ii{1, 2};
	int zero = 0;
	int one = 1;
	BOOST_CHECK((*at)(t_ii, zero) == 1);
	BOOST_CHECK((*at)(t_ii, one) == 2);
	
	/* at() with runtime values for tuples with different types (std::tuple<int, float>) will not work, 
	 * because return type of at() would be different for tuples depending on offset.
	 * Example:
	 *  template<typename T>
	 *  constexpr decltype(auto)
	 *  at_2(T && t, std::size_t && i) {
	 *    return i == 0 ? std::get<0>(t) : std::get<1>(t);
	 *  }
	 *  struct A{};
	 *  static_assert(at_2(std::tuple<int, A>{1234, A{}}, 0) == 1234, "");
	 * will result in error: incompatible operand types ('int' and 'A')
	 */
	
	static_assert((*at)(hana::make_tuple(42_c, 666_c, 1337_c), 2_c) == 1337_c, "");
	
	static_assert((*at)(hana::make_tuple(1337_c, 1337_c, 1337_c), 2) == 1337_c, "");
	
	static_assert((*at)(hana::make_tuple(42_c, 666_c, 1337_c), std::integral_constant<int, 2>{}) == 1337_c, "");
	
	std::vector<int> v1{1337,42, 666};
	BOOST_CHECK((*at)(v1, 1) == 42);
	
	BOOST_CHECK((*at)(std::array<int, 3>{{1337, 42, 666}}, 1_c) == 42);
	
	BOOST_CHECK(detail::at_impl_std_array{}(std::array<int, 3>{{1337, 42, 666}}, 1) == 42);
	BOOST_CHECK((*at)(std::array<int, 3>{{1337, 42, 666}}, 1) == 42);
	
	BOOST_TEST((*at)(boost::irange(5,11), 2) == 7);
	auto const r1 = boost::irange(5,11);
	BOOST_TEST((*at)(r1, 2) == 7);
}

BOOST_AUTO_TEST_SUITE_END()
