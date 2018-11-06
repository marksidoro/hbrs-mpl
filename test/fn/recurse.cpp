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


#define BOOST_TEST_MODULE recurse_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/plus.hpp>
#include <boost/hana/minus.hpp>
#include <boost/hana/greater.hpp>
#include <boost/hana/equal.hpp>


#include <boost/hana/type.hpp>
#include <hbrs/mpl/fn/recurse.hpp>

#include <type_traits>
#include <tuple>

BOOST_AUTO_TEST_SUITE(recurse_test)

using namespace hbrs::mpl;

struct Condition {
	template<typename I, typename S, typename J>
	constexpr decltype(auto)
	operator()(I const& i, S const& s, J const& j) const{ 
		return (j == hana::int_c<1>);
	}
};

struct LastStep {
	template<typename I, typename S, typename J>
	constexpr decltype(auto)
	operator()(I const& i, S const& s, J const& j) const { 
		return hana::plus(s, i);
	}
};

struct RecursionStep {
	template<typename I, typename S, typename J>
	constexpr decltype(auto)
	operator()(I const& i, S const& s, J const& j) const { 
		return std::make_tuple(i, hana::plus(s,i), j-hana::int_c<1>);
	}
};

BOOST_AUTO_TEST_CASE(recurse_test_1) {
	using namespace hbrs::mpl;
	
	BOOST_CHECK((*recurse)(Condition{}, LastStep{}, RecursionStep{})(2,0,10) == 20);
	static_assert((*recurse)(Condition{}, LastStep{}, RecursionStep{})(hana::int_c<2>, hana::int_c<0>, hana::int_c<10>) == hana::int_c<20>, "");
	
	BOOST_CHECK(
		(*recurse)(
			[](auto a, auto b, auto c) { return (a == 1); },
			[](auto a, auto b, auto c) { return a+b; }, 
			[](auto a, auto b, auto c) { return std::make_tuple(a, a+b, c-1); }
		)(2,0,10) == 20
	);
}

BOOST_AUTO_TEST_SUITE_END()
