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


#define BOOST_TEST_MODULE is_braces_constructible_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/equal.hpp>
#include <hbrs/mpl/detail/is_braces_constructible.hpp>
#include <string>
#include <vector>

BOOST_AUTO_TEST_SUITE(is_braces_constructible_test)

struct dummy{};
struct ctor_test1{
	constexpr
	ctor_test1(dummy &){};
};

BOOST_AUTO_TEST_CASE(is_braces_constructible_test_1) {
	using namespace hbrs::mpl;
	static_assert(!detail::is_braces_constructible<std::string, dummy>::value, "");
	static_assert( detail::is_braces_constructible<std::string, char const*>::value, "");
	static_assert( detail::is_braces_constructible<std::string, std::string>::value, "");
	static_assert(!detail::is_braces_constructible<int, double>::value, "");
	static_assert(!detail::is_braces_constructible<double, int>::value, "");
	
	static_assert(!detail::is_braces_constructible<ctor_test1, dummy>::value, "");
	static_assert( detail::is_braces_constructible<ctor_test1, dummy&>::value, "");
	
	static_assert( detail::is_braces_constructible<dummy&, dummy&>::value, "");
	static_assert( detail::is_braces_constructible<int&, int&>::value, "");
	
	static_assert( detail::is_braces_constructible<int, int>::value, "");
	static_assert( detail::is_braces_constructible<dummy&&, dummy>::value, "");
	static_assert( detail::is_braces_constructible<dummy, dummy&&>::value, "");
	static_assert( detail::is_braces_constructible<std::vector<double>, std::vector<double>&&>::value, "");
	
	static_assert( detail::is_braces_constructible<dummy const&, dummy>::value, "");
	static_assert( detail::is_braces_constructible<dummy const&, dummy &>::value, "");
	static_assert( detail::is_braces_constructible<dummy const&, dummy const&>::value, "");
	static_assert( detail::is_braces_constructible<dummy const&, dummy &&>::value, "");
	
	dummy a;
	dummy & b{a};
	dummy c{b};
	
	dummy const& d{a};
	dummy e{d};
	
	static_assert( detail::is_braces_constructible<dummy, dummy>::value, "");
	static_assert( detail::is_braces_constructible<dummy, dummy &>::value, "");
	static_assert( detail::is_braces_constructible<dummy, dummy const&>::value, "");
	static_assert( detail::is_braces_constructible<dummy, dummy &&>::value, "");
}

BOOST_AUTO_TEST_SUITE_END()
