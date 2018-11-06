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


#define BOOST_TEST_MODULE invoke_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/fn/invoke.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/back.hpp>
#include <boost/hana/front.hpp>
#include <tuple>

BOOST_AUTO_TEST_SUITE(invoke_test)

namespace hana = boost::hana;

struct t1_t {
	
	constexpr
	t1_t(t1_t const&) noexcept = delete;
	
	constexpr
	t1_t(t1_t &&) noexcept = default;
	
	constexpr t1_t&
	operator=(t1_t const&) noexcept = delete;
	
	constexpr t1_t&
	operator=(t1_t &&) noexcept = delete;
	
};

struct t2_t {
	constexpr decltype(auto)
	operator()(int, double, t1_t &&) const {
		return hana::int_c<1337>;
	}
};

BOOST_AUTO_TEST_CASE(invoke_test_1) {
	using namespace hbrs::mpl;
	
	static_assert(t2_t{}(1, 2.0, t1_t{}) == hana::int_c<1337>, "");
	static_assert((*invoke)(t2_t{}, 1, 2.0, t1_t{}) == hana::int_c<1337>, "");
	
	constexpr hana::int_<1337> s55 =
		(*invoke)(
			hana::id,
			invoke(
				hana::front, 
				hana::make_tuple(hana::int_c<1337>)
			)
		);
	
	static_assert(std::is_same<
		decltype(hana::front(hana::make_tuple(hana::int_c<1337>))),
		hana::int_<1337> &&
	>{}, "");
	static_assert(std::is_same<
		decltype(invoke(hana::front, hana::make_tuple(hana::int_c<1337>))),
		expression<
			invoke_t, 
			hana::tuple<
				detail::lvalue_reference_wrapper<hana::front_t const>,
				detail::rvalue_reference_wrapper<
					hana::tuple< hana::int_<1337> >
				>
			>
		> 
	>{}, "");
	
	static_assert(std::is_same<
		decltype(hana::back(invoke(hana::front, hana::make_tuple(hana::int_c<1337>)).operands())),
		detail::rvalue_reference_wrapper<
			hana::tuple<
				hana::integral_constant<int, 1337> 
			>
		> &&
	>{}, "");
	
	
	static_assert(std::is_same<
		decltype((*invoke)(hana::front, hana::make_tuple(hana::int_c<1337>))),
		hana::int_<1337>
	>{}, "");
	
	static_assert(std::is_same<
		decltype(hana::id((*invoke)(hana::front, hana::make_tuple(hana::int_c<1337>)))),
		hana::int_<1337>
	>{}, "");
	
	static_assert(std::is_same<
		decltype(wrap_reference((*invoke)(hana::front, hana::make_tuple(hana::int_c<1337>)))),
		detail::rvalue_reference_wrapper< hana::int_<1337> >
	>{}, "");
	
	static_assert(std::is_same<
		decltype(hana::id(unwrap_reference((*invoke)(hana::front, hana::make_tuple(hana::int_c<1337>))))),
		hana::int_<1337>
	>{}, "");
	
}

BOOST_AUTO_TEST_SUITE_END()
