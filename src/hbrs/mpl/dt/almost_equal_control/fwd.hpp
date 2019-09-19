/* Copyright (c) 2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_DT_ALMOST_EQUAL_CONTROL_FWD_HPP
#define HBRS_MPL_DT_ALMOST_EQUAL_CONTROL_FWD_HPP

#include <hbrs/mpl/config.hpp>
#include <boost/hana/fwd/core/make.hpp>
#include <boost/hana/fwd/core/to.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;

template<typename MaxULPsDiff, typename AlmostZeroPrecision>
struct almost_equal_control;
struct almost_equal_control_tag {};
constexpr auto make_almost_equal_control = hana::make<almost_equal_control_tag>;
constexpr auto to_almost_equal_control = hana::to<almost_equal_control_tag>;

HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DT_ALMOST_EQUAL_CONTROL_FWD_HPP
