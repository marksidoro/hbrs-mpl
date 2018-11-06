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


#define BOOST_TEST_MODULE apply_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/fn/apply.hpp>
#include <hbrs/mpl/core/evaluate.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/front.hpp>
#include <boost/hana/tuple.hpp>
#include <tuple>

BOOST_AUTO_TEST_SUITE(apply_test)

struct t1_t {
	
	constexpr
	t1_t(t1_t const&) noexcept = delete;
	
	constexpr
	t1_t(t1_t &&) noexcept = delete;
	
	constexpr t1_t&
	operator=(t1_t const&) noexcept = delete;
	
	constexpr t1_t&
	operator=(t1_t &&) noexcept = delete;
	
};

struct t2_t {
	constexpr decltype(auto)
	operator()(int, double, t1_t &&) const {
		return boost::hana::int_c<1337>;
	}
};

BOOST_AUTO_TEST_CASE(apply_test_1) {
	using namespace hbrs::mpl;
	
// 	static_assert((*apply)(t2_t{}, std::forward_as_tuple(1, 2.0, t1_t{})) == hana::int_c<1337>, "");
	static_assert((*apply)(t2_t{}, hana::tuple<int, double, t1_t&&>{1, 2.0, t1_t{}}) == hana::int_c<1337>, "");
	
	static_assert(std::is_same<
		decltype(hana::front(hana::make_tuple(hana::int_c<1337>))),
		hana::int_<1337> &&
	>{}, "");
	
	static_assert(std::is_same<
		decltype(detail::apply_impl_hana_tuple_unpack{}(
			hana::front, 
			hana::make_tuple(hana::make_tuple(hana::int_c<1337>))
		)),
		hana::int_<1337> &&
	>{}, "");
	
	static_assert(std::is_same<
		decltype(hana::make_tuple(detail::apply_impl_hana_tuple_unpack{}(
			hana::front, 
			hana::make_tuple(hana::make_tuple(hana::int_c<1337>)))
		)),
		hana::tuple<hana::int_<1337>>
	>{}, "");
	
}

BOOST_AUTO_TEST_SUITE_END()
