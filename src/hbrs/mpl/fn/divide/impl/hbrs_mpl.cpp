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

#include "hbrs_mpl.hpp"

#include <hbrs/mpl/dt/rtsacv.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

template
decltype(auto)
divide_impl_rtsacv_scalar::operator()(rtsacv<float> const&, float const&) const;

template
decltype(auto)
divide_impl_rtsacv_scalar::operator()(rtsacv<double> const&, double const&) const;

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

