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

#define BOOST_TEST_MODULE error_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/core/is_error_cause.hpp>
#include <hbrs/mpl/dt/stack_frame.hpp>
#include <hbrs/mpl/dt/error.hpp>

#include <boost/hana/tuple.hpp>

struct op1_t {};
constexpr op1_t op1{};

// template<typename T>
// struct t1 {
// 	
// 	constexpr
// 	t1(t1 const&) = delete;
// 	
// 	constexpr t1
// 	operator=(t1 const&) {};
// 	
// 	constexpr
// 	operator boost::hana::int_<1337>() const { return {};  }
// 	
// 	constexpr
// 	operator auto() const { return boost::hana::int_c<42>;  }
// };


// namespace hana = boost::hana;
// 
// struct t2{
// 	template<typename T>
// 	constexpr auto
// 	method(T) {
// 		return hana::int_c<1337>;
// 	}
// };


BOOST_AUTO_TEST_SUITE(error_test)

BOOST_AUTO_TEST_CASE(error_test_1) {
	using namespace hbrs::mpl;
	namespace hana = boost::hana;
	
	static_assert(is_error_cause(make_error(error_cause::not_implemented{}), error_cause::not_implemented{}), "");
	static_assert(!is_error_cause(make_error(error_cause::not_implemented{}), error_cause::recursion_detected{}), "");
	static_assert(is_error_cause(make_stack_frame(make_error(error_cause::not_implemented{}), op1, hana::make_tuple()), error_cause::not_implemented{}), "");
	static_assert(!is_error_cause(make_stack_frame(make_error(error_cause::not_implemented{}), op1, hana::make_tuple()), error_cause::recursion_detected{}), "");
	
// 	t1<int> a{};
// 	auto b = t1<int>{};
}

BOOST_AUTO_TEST_SUITE_END()
