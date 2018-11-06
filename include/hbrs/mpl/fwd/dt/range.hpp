/* Copyright (c) 2018 Jakob Meng, <jakobmeng@web.de>
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

#pragma once

#ifndef HBRS_MPL_FWD_DT_RANGE_HPP
#define HBRS_MPL_FWD_DT_RANGE_HPP

#include <hbrs/mpl/config.hpp>
#include <boost/hana/fwd/core/make.hpp>
#include <boost/hana/fwd/core/to.hpp>
#include <ostream>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;

template<typename First, typename Last>
struct range;
struct range_tag{};
constexpr auto make_range = hana::make<range_tag>;
constexpr auto to_range = hana::to<range_tag>;

template<typename First, typename Last>
std::ostream& operator<<(std::ostream& os, range<First, Last> const& rng);

HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FWD_DT_RANGE_HPP
