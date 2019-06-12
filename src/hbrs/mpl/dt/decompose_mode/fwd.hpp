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

#ifndef HBRS_MPL_FWD_DT_DECOMPOSE_MODE_HPP
#define HBRS_MPL_FWD_DT_DECOMPOSE_MODE_HPP

#include <hbrs/mpl/config.hpp>
#include <boost/hana/fwd/core/make.hpp>
#include <boost/hana/fwd/core/to.hpp>
#include <boost/hana/integral_constant.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;

enum class decompose_mode { complete, economy, zero};
template <decompose_mode dm>
using decompose_mode_ = hana::integral_constant<decompose_mode, dm>; 
template <decompose_mode dm>
constexpr decompose_mode_<dm> decompose_mode_c{};
using complete_decompose_ = decompose_mode_<decompose_mode::complete>;
constexpr auto complete_decompose_c = decompose_mode_c<decompose_mode::complete>;
using economy_decompose_ = decompose_mode_<decompose_mode::economy>;
constexpr auto economy_decompose_c = decompose_mode_c<decompose_mode::economy>;
using zero_decompose_ = decompose_mode_<decompose_mode::zero>;
constexpr auto zero_decompose_c = decompose_mode_c<decompose_mode::zero>;

HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FWD_DT_DECOMPOSE_MODE_HPP
