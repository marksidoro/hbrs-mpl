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


#define BOOST_TEST_MODULE if_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/core/preprocessor.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/plus.hpp>
#include <boost/hana/minus.hpp>
#include <boost/hana/less.hpp>
#include <boost/hana/equal.hpp>


#include <boost/hana/type.hpp>
#include <hbrs/mpl/fn/if.hpp>
#include <hbrs/mpl/fn/invoke.hpp>

#include <type_traits>
#include <tuple>

BOOST_AUTO_TEST_SUITE(if_test)

template<typename Bool, typename If, typename Else>
struct functor_t {
	constexpr
	functor_t(Bool const b, If i, Else e) : b{b}, i{i}, e{e} {};
	
	template <typename... Args>
	constexpr decltype(auto)
	operator()(Args&& ...args) {
		return b ? i(HBRS_MPL_FWD(args)...) : e(HBRS_MPL_FWD(args)...);
	}
	
	template <typename... Args>
	constexpr decltype(auto)
	operator()(Args&& ...args) const {
		return b ? i(HBRS_MPL_FWD(args)...) : e(HBRS_MPL_FWD(args)...);
	}
	
	Bool const b;
	If i;
	Else e;
};

BOOST_AUTO_TEST_CASE(if_test_1) {
	using namespace hbrs::mpl;
	
	auto if1 = (*if_)(
		true, 
		[](){ return 1; }, 
		[](){ return 2; }
	);
	BOOST_ASSERT(if1() == 1);
	
	struct IfCase1 {
		constexpr decltype(auto)
		operator()() const { return 1; }
	};
	
	struct ElseCase1 {
		constexpr decltype(auto)
		operator()() const { return 2; }
	};
	
	struct ElseCase2 {
		constexpr decltype(auto)
		operator()() const { return ' '; }
	};
	
	static constexpr auto if2 = (*if_)(
		hana::true_c,
		IfCase1{}, 
		ElseCase2{}
	);
	
	static constexpr auto if3 = (*if_)(
		hana::false_c,
		IfCase1{}, 
		ElseCase2{}
	);
	
	static_assert(if2() == 1, "");
	static_assert(if3() == ' ', "");
	
	static constexpr auto if4 = (*if_)(
		std::true_type{},
		IfCase1{}, 
		ElseCase2{}
	);
	
	static constexpr auto if5 = (*if_)(
		std::false_type{},
		IfCase1{}, 
		ElseCase2{}
	);
	
	static_assert(if4() == 1, "");
	static_assert(if5() == ' ', "");
	
	auto f1 = functor_t<bool, IfCase1, ElseCase1>{true, IfCase1{}, ElseCase1{}};
	auto f2 = functor_t<bool, IfCase1, ElseCase1>{false, IfCase1{}, ElseCase1{}};
	auto if6 = (*if_)(true, f1, f2);
	auto if7 = (*if_)(false, f1, f2);
	BOOST_CHECK(if6() == 1);
	BOOST_CHECK(if7() == 2);
	
	BOOST_CHECK((*if_)(true, 1, 2) == 1);
	BOOST_CHECK((*if_)(false, 1, 2) == 2);
	
	//NOTE: Without lazy evaluation of both if branches those statements will fail to compile, 
	//      because hana::front is invoked early on empty std::tuple!
// 	static_assert(std::is_same<
// 		decltype(detail::if_impl_hana_bool{}(
// 			hana::true_c, 
// 			invoke(hana::front, hana::make_tuple(hana::int_c<1337>)), 
// 			invoke(hana::front, std::make_tuple())
// 		)),
// 		expression<
// 			invoke_t,
// 			hana::tuple<
// 				detail::lvalue_reference_wrapper<const hana::front_t>,
// 				detail::rvalue_reference_wrapper<
// 					hana::tuple< hana::integral_constant<int, 1337> >
// 				>
// 			>
// 		> &&
// 	>{}, "");
// 	
// 	constexpr hana::int_<1337> if8 = (*if_)(
// 		hana::true_c, 
// 		invoke(hana::front, hana::make_tuple(hana::int_c<1337>)), 
//         invoke(hana::front, hana::make_tuple())
// 	);
}

BOOST_AUTO_TEST_SUITE_END()
