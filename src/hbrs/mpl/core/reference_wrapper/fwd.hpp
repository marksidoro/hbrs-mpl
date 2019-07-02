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

#ifndef HBRS_MPL_CORE_REFERENCE_WRAPPER_FWD_HPP
#define HBRS_MPL_CORE_REFERENCE_WRAPPER_FWD_HPP

#include <hbrs/mpl/config.hpp>
#include <boost/hana/fwd/core/make.hpp>
#include <boost/hana/fwd/core/to.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

//TODO: move lvalue_reference_wrapper to detail folder or move from hbrs::mpl::detail namespace to hbrs::mpl
template <typename T>
struct lvalue_reference_wrapper;
struct lvalue_reference_wrapper_tag{};
constexpr auto make_lvalue_reference_wrapper = hana::make<lvalue_reference_wrapper_tag>;
constexpr auto to_lvalue_reference_wrapper = hana::to<lvalue_reference_wrapper_tag>;

//TODO: move rvalue_reference_wrapper to detail folder or move from hbrs::mpl::detail namespace to hbrs::mpl
template <typename T>
struct rvalue_reference_wrapper;
struct rvalue_reference_wrapper_tag{};
constexpr auto make_rvalue_reference_wrapper = hana::make<rvalue_reference_wrapper_tag>;
constexpr auto to_rvalue_reference_wrapper = hana::to<rvalue_reference_wrapper_tag>;

/* namespace detail */ }

HBRS_MPL_DEC_CF1(unwrap_reference)
HBRS_MPL_DEC_CF1(unwrap_references)
HBRS_MPL_DEC_CF1(wrap_reference)
HBRS_MPL_DEC_CFV(wrap_references)

HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_CORE_REFERENCE_WRAPPER_FWD_HPP
