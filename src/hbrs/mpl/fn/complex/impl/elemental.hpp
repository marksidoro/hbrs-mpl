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

#ifndef HBRS_MPL_FN_COMPLEX_IMPL_ELEMENTAL_HPP
#define HBRS_MPL_FN_COMPLEX_IMPL_ELEMENTAL_HPP

#include "../fwd/elemental.hpp"
#ifdef HBRS_MPL_ENABLE_ELEMENTAL

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>

#include <hbrs/mpl/dt/el_vector.hpp>
#include <hbrs/mpl/dt/el_dist_vector.hpp>

#include <hbrs/mpl/fn/at.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {


template <typename Ring>
auto
complex_impl_el_vector::operator()(
	el_column_vector<Ring> const& real,
	Ring const& imag
) const {
	typedef std::decay_t<Ring> _Ring_;
	el_column_vector<El::Complex<_Ring_>> v = {real.length()};
	for (El::Int i = 0; i < real.length(); ++i) {
		v.at(i) = El::Complex<_Ring_>{real.at(i), imag};
	}
	
	return v;
}

template <typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
auto
complex_impl_el_dist_vector::operator()(
	el_dist_column_vector<Ring, Columnwise, Rowwise, Wrapping> const& real,
	Ring const& imag
) const {
	typedef std::decay_t<Ring> _Ring_;
	el_dist_column_vector<El::Complex<_Ring_>, Columnwise, Rowwise, Wrapping> v = {real.data().Grid(), real.length()};
	// TODO: Optimize!
	for (El::Int i = 0; i < real.length(); ++i) {
		v.data().Set(i, 0, El::Complex<_Ring_>{real.data().Get(i, 0), imag});
	}
	
	return v;
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
#endif // !HBRS_MPL_FN_COMPLEX_IMPL_ELEMENTAL_HPP
