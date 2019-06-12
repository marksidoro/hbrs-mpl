/* Copyright (c) 2016-2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_FWD_DT_SVD_RESULT_HPP
#define HBRS_MPL_FWD_DT_SVD_RESULT_HPP

#include <hbrs/mpl/config.hpp>
#include <boost/hana/fwd/core/make.hpp>
#include <boost/hana/fwd/core/to.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;

template<typename U, typename S, typename V>
struct svd_result;
struct svd_result_tag {};
constexpr auto make_svd_result = hana::make<svd_result_tag>;
constexpr auto to_svd_result = hana::to<svd_result_tag>;

struct svd_u;
struct svd_s;
struct svd_v;

HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FWD_DT_SVD_RESULT_HPP
