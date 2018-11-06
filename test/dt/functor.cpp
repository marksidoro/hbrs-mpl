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

#define BOOST_TEST_MODULE functor_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/dt/functor.hpp>

BOOST_AUTO_TEST_SUITE(functor_test)

constexpr decltype(auto)
test1(int v) {
	return v;
}

template<typename T>
constexpr decltype(auto)
test2(T t) {
	return t;
}

struct test3 {
	template <typename T>
	constexpr decltype(auto)
	operator()(T t) const {
		return t;
	}
};

template<typename>
struct test4 {
	template <typename T>
	constexpr decltype(auto)
	operator()(T t) const {
		return t;
	}
};

BOOST_AUTO_TEST_CASE(functor_test_1) {
	using namespace hbrs::mpl;

	constexpr auto t1 = make_functor(test1);
	static_assert(t1(1337) == 1337, "");
	
	/* FIXME: Workaround for GCC6 bug: If test2<> is not used in any way before using make_functor() then GCC fails with:
	 *        "error: no match for call to '(const boost::hana::make_t<hbrs::mpl::functor_tag>) (<unresolved overloaded 
	 *         function type>)'"
	 */
	typedef decltype(test2<int>) gcc_workaround; 
	
	constexpr auto t2 = make_functor(test2<int>);
	static_assert(t2(42) == 42, "");
	
	constexpr auto t3 = make_functor(test3{});
	static_assert(t3(4711) == 4711, "");
	
	constexpr auto t4 = make_functor(test4<test3>{});
	static_assert(t4(11) == 11, "");
}

BOOST_AUTO_TEST_SUITE_END()
