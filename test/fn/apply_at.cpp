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


#define BOOST_TEST_MODULE apply_at_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <boost/hana/type.hpp>
#include <boost/hana/tuple.hpp>
#include <hbrs/mpl/core/evaluate.hpp>
#include <hbrs/mpl/dt/ctsav.hpp>
#include <hbrs/mpl/dt/rtsav.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/apply_at.hpp>

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/functional/reverse_partial.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/equal.hpp>

#include <tuple>
#include <array>
#include <type_traits>

BOOST_AUTO_TEST_SUITE(apply_at_test)
using namespace hbrs::mpl;

template<std::size_t Integral>
constexpr std::integral_constant<std::size_t, Integral> ic_c{};

constexpr auto first_at = hana::reverse_partial(at, ic_c<0>);
constexpr auto second_at = hana::reverse_partial(at, ic_c<1>);

BOOST_AUTO_TEST_CASE(apply_at_test_1) {
	using namespace hbrs::mpl;
	
	typedef std::tuple<int> _tuple_i;
	typedef std::tuple<int, int> _tuple_ii;
	typedef std::tuple< _tuple_i const&, _tuple_ii const&> _tuple_i_ii;
	
	constexpr static auto t_i = _tuple_i{1};
	constexpr static auto t_ii = _tuple_ii{2, 3};
	constexpr static auto t_i_ii = _tuple_i_ii{ t_i, t_ii };
	
	static_assert(evaluate(first_at(t_i_ii)) == t_i, "");
	static_assert(evaluate(second_at(t_i_ii)) == t_ii, "");
	//TODO
// 	static_assert((*at)(at(t_i_ii, ic_c<1>), ic_c<0>) == 2, "");
// 	static_assert(evaluate(first_at(second_at(t_i_ii))) == 2, "");
	
	static_assert(detail::apply_at_impl_std_tuple_ic{}(hana::id, t_i_ii, ic_c<0>) == t_i, "");
	static_assert((*apply_at)(hana::id, t_i_ii, ic_c<0>) == t_i, "");
	
	using namespace hana::literals;
	static_assert((*apply_at)(hana::id, t_i_ii, 1_c ) == t_ii, "");
	static_assert((*apply_at)(first_at, t_i_ii, 0_c ) == 1, "");
	static_assert((*apply_at)(first_at, t_i_ii, 1_c ) == 2, "");
	static_assert((*apply_at)(second_at, t_i_ii, 1_c ) == 3, "");
	
	typedef std::tuple< _tuple_i&, _tuple_ii&> _tuple_i_ii_rt;
	
	auto t_i_rt = _tuple_i{1};
	auto t_ii_rt = _tuple_ii{2, 3};
	auto t_i_ii_rt = _tuple_i_ii_rt{ t_i_rt, t_ii_rt };
	
	BOOST_CHECK(detail::apply_at(hana::id, t_i_rt, 1) == 4);
	static_assert(hana::Sequence<std::tuple<int>&>::value, "");
	BOOST_CHECK(detail::at_impl_std_tuple_hmg_integral{}(t_i_rt, 1_c) == 4);
	(*apply_at)(first_at, t_i_ii_rt, 1_c) = 4;
	BOOST_CHECK((*at)(t_ii_rt, 0_c) == 4);
	BOOST_CHECK(detail::apply_at(hana::id, t_ii_rt, 0) == 4);
	BOOST_CHECK(detail::apply_at_impl_std_tuple_integral{}(hana::id, t_ii_rt, 0) == 4);
	BOOST_CHECK((*apply_at)(hana::id, t_ii_rt, 0) == 4);
	
	int zero = 0;
	BOOST_CHECK((*apply_at)(hana::id, t_ii_rt, zero) == 4);
	
	constexpr static auto t_ii_ct_1 = _tuple_ii{1, 2};
	constexpr static auto t_ii_ct_2 = _tuple_ii{3, 4};
	constexpr static auto t_ii_ct_3 = _tuple_ii{5, 6};
	constexpr static std::array<_tuple_ii, 3> a_3_ct {{ t_ii_ct_1, t_ii_ct_2, t_ii_ct_3 }};
	
	static_assert((*apply_at)(hana::id, a_3_ct, 0_c) == t_ii_ct_1, "");
	static_assert((*apply_at)(hana::id, a_3_ct, 1_c) == t_ii_ct_2, "");
	static_assert((*apply_at)(hana::id, a_3_ct, 2_c) == t_ii_ct_3, "");
	
	static_assert((*apply_at)(first_at, a_3_ct, 1_c) == 3, "");
	static_assert((*apply_at)(second_at, a_3_ct, 2_c) == 6, "");
	
	auto t_i_rt_1 = _tuple_i{1};
	auto t_i_rt_2 = _tuple_i{2};
	auto t_i_rt_3 = _tuple_i{3};
	std::array<_tuple_i, 3> a_3_rt {{ t_i_rt_1, t_i_rt_2, t_i_rt_3 }};
	
	BOOST_CHECK((*apply_at)(hana::id, a_3_rt, 0) == t_i_rt_1);
	BOOST_CHECK((*apply_at)(hana::id, a_3_rt, 1) == t_i_rt_2);
	BOOST_CHECK((*apply_at)(hana::id, a_3_rt, 2) == t_i_rt_3);
	
	auto first_at0 = hana::reverse_partial(hana::partial(detail::apply_at, hana::id), zero);
	BOOST_CHECK(first_at0(t_i_rt_1) == 1);
	BOOST_CHECK(first_at0(t_i_rt_2) == 2);
	
	auto first_at1 = hana::compose(evaluate, hana::reverse_partial(at, zero));
	BOOST_CHECK(first_at1(t_i_rt_1) == 1);
	BOOST_CHECK(first_at1(t_i_rt_2) == 2);
	
	BOOST_CHECK((*apply_at)(first_at, a_3_rt, 2) == 3);
	
	static_assert(detail::apply_at(hana::front, hana::make_tuple(hana::make_tuple(1, 2, 3), hana::make_tuple(4), hana::make_tuple(5, 6)), 1) == 4, "");
	
	static_assert(
		hana::type_c<decltype(
			(*apply_at)(hana::id, hana::make_tuple(1, 2.0, 3_c), 2_c)
		)> == hana::type_c<
			hana::llong<3>
		>, "");
	
	static_assert(
		hana::type_c<decltype(
			(*apply_at)(hana::id, hana::make_tuple(1, 2.0, 3_c), ic_c<2>)
		)> == hana::type_c<
			hana::llong<3>
		>, "");
}

BOOST_AUTO_TEST_SUITE_END()
