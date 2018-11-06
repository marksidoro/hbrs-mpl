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

#include <hbrs/mpl/fn/power.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

template decltype(auto) power_impl_std_arithmetic::operator()(float, float) const;
template decltype(auto) power_impl_std_arithmetic::operator()(double, double) const;
template decltype(auto) power_impl_std_arithmetic::operator()(long double, long double) const;

template decltype(auto) power_impl_std_complex::operator()(std::complex<float> const&, std::complex<float> const&) const;
template decltype(auto) power_impl_std_complex::operator()(std::complex<double> const&, std::complex<double> const&) const;
template decltype(auto) power_impl_std_complex::operator()(std::complex<long double> const&, std::complex<long double> const&) const;

template decltype(auto) power_impl_std_complex::operator()(std::complex<float> const&, float const&) const;
template decltype(auto) power_impl_std_complex::operator()(std::complex<double> const&, double const&) const;
template decltype(auto) power_impl_std_complex::operator()(std::complex<long double> const&, long double const&) const;

template decltype(auto) power_impl_std_complex::operator()(float const&, std::complex<float> const&) const;
template decltype(auto) power_impl_std_complex::operator()(double const&, std::complex<double> const&) const;
template decltype(auto) power_impl_std_complex::operator()(long double const&, std::complex<long double> const&) const;

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END
