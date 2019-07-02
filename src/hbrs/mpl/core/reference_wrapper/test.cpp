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

#define BOOST_TEST_MODULE reference_wrapper_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/core/reference_wrapper.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/front.hpp>
#include <boost/hana/functional/id.hpp>
#include <vector>
#include <type_traits>

struct t4 {
	constexpr
	t4() {};
	
	constexpr
	t4(t4 const&) = delete;
	
	constexpr
	t4(t4 &&) = delete;
};

BOOST_AUTO_TEST_SUITE(reference_wrapper_test)

BOOST_AUTO_TEST_CASE(reference_wrapper_test_1) {
	using namespace hbrs::mpl;
	
	wrap_references(true);
	
	t4 s58{};
// 	decltype(auto) s59 = hana::id(t6{});
// 	static_assert(std::is_same<
// 		decltype(s59), t6
// 	>{}, "");
	
// 	constexpr t6 s75 = hana::at_c<0>(hana::make_tuple(t6{}));
	
// 	constexpr t6 s76 = hana::at_c<0>(hana::id(hana::make_tuple(t6{})));
	
	
	
	
	detail::lvalue_reference_wrapper<t4> s60 = detail::make_lvalue_reference_wrapper(s58);
	detail::lvalue_reference_wrapper<t4> s61{s60};
	BOOST_CHECK(&(s60.get()) == &(s61.get()));
	detail::lvalue_reference_wrapper<t4> s63{hana::id(s60)};
	BOOST_CHECK(&(s63.get()) == &s58);
	
	BOOST_CHECK(&(hana::id(s63).get()) == &s58);
	
	BOOST_CHECK(&(wrap_reference(s60).get()) == &s58);
	
	static_assert(std::is_same<
		decltype(unwrap_reference(detail::make_rvalue_reference_wrapper(4))),
		int&&
	>{}, "");
	
	
// 	static_assert(std::is_same<
// 		decltype(wrap_reference(t4{})), t4&&
// 	>{}, "");
	
	/* hana::id(t4{}) => not allowed because t4 has no move constructor! */


	
	constexpr detail::lvalue_reference_wrapper< hana::int_<1337> const > s38 = wrap_reference(hana::int_c<1337>);
	
	static_assert(std::is_same<
		decltype(wrap_references(4)),
		hana::tuple< detail::rvalue_reference_wrapper<int> >
	>{}, "");
	constexpr hana::tuple< detail::rvalue_reference_wrapper<int> > s64 = wrap_references(4);
	BOOST_CHECK(hana::at_c<0>(s64) == 4);
	
	
	constexpr hana::tuple< int > s65 = unwrap_references(wrap_references(4));
	BOOST_CHECK(hana::at_c<0>(s65) == 4);
	
	static_assert(std::is_same<
		decltype(wrap_reference(hana::int_<1337>{})),
		detail::rvalue_reference_wrapper< hana::int_<1337> >
	>{}, "");

	static_assert(std::is_same<
		decltype(unwrap_reference(wrap_reference(hana::int_c<1337>))),
		hana::int_<1337> const &
	>{}, "");
	
	static_assert(std::is_same<
		decltype(unwrap_reference(wrap_reference(hana::int_<1337>{}))),
		hana::int_<1337> &&
	>{}, "");
	
	static_assert(std::is_same<
		decltype(wrap_reference(hana::int_<1337>{})),
		detail::rvalue_reference_wrapper< hana::int_<1337> >
	>{}, "");
	
	static_assert(std::is_same<
		decltype(hana::int_<1337>{}),
		hana::int_<1337>
	>{}, "");
	
	
	static_assert(std::is_same<
		decltype(unwrap_reference(s38)),
		hana::int_<1337> const &
	>{}, "");
	
	constexpr auto s39 = detail::make_lvalue_reference_wrapper(hana::int_c<1337>);
	constexpr auto s40 = detail::make_rvalue_reference_wrapper(hana::int_<1337>{});
	constexpr auto s41 = s39;
	constexpr auto s42 = s40;
	
	static_assert(&unwrap_reference(wrap_reference(hana::front)) == &hana::front, "");
	
	auto ref_qualifier_test1 = [](auto && ref) {
		auto drop = HBRS_MPL_FWD(ref).get();
		return HBRS_MPL_FWD(ref);
	};
	BOOST_TEST(ref_qualifier_test1(detail::rvalue_reference_wrapper<std::vector<int>>{ std::vector<int>{1,2,3} }).get().empty());
	
	auto s1 = detail::rvalue_reference_wrapper<std::vector<int>>{ std::vector<int>{1,2,3} };
	BOOST_TEST(!ref_qualifier_test1(s1).get().empty());
}

BOOST_AUTO_TEST_SUITE_END()
