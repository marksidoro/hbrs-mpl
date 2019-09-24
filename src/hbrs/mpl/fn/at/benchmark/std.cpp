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

#include <cstdlib>

#include <type_traits>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>

#include <tuple>
#include <hbrs/mpl/fn/at.hpp>

#include <cassert>
#include <iostream>

namespace hana = boost::hana;
using namespace hana::literals;
using namespace hbrs::mpl;

int
main(int argc, char **argv) {
	constexpr auto r0 = (*at)(std::tuple<int>{1337}, 0_c);
	static_assert(r0 == 1337, "");
	
	auto r1 = (*at)(std::tuple<int>{1337}, 0_c);
	assert(r1 == 1337);
	
	std::cout << r1 << std::endl;
	
	auto r2 = (*at)(std::tuple<int>{1337}, 0);
	assert(r2 == 1337);
	
	std::cout << r2 << std::endl;
	
	return EXIT_SUCCESS;
}