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

#ifndef HBRS_MPL_FN_INVERSE_IMPL_ELEMENTAL_HPP
#define HBRS_MPL_FN_INVERSE_IMPL_ELEMENTAL_HPP

#include "../fwd/elemental.hpp"
#ifdef HBRS_MPL_ENABLE_ELEMENTAL

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>

#include <hbrs/mpl/dt/el_matrix.hpp>
#include <hbrs/mpl/dt/el_dist_matrix.hpp>

#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/equal.hpp>
#include <hbrs/mpl/fn/divide.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {
namespace inverse_el {

template<typename Matrix, typename Inverse>
static auto
inverse(Matrix const& x, Inverse && inv) {
	typedef decltype(x.data.data().Get(0,0)) Ring;
	typedef std::decay_t<Ring> _Ring_;
	
	auto sz_ = (*size)(x.data);
	auto m_ = (*m)(sz_);
	auto n_ = (*n)(sz_);
	
	BOOST_ASSERT((*equal)(m_, n_));
	
	inv.data().Resize(m_, n_);
	
	// TODO: Replace with faster algorithm!
	for(El::Int j = 0; j<n_; ++j) {
		auto v_jj = x.data.data().Get(j,j);
		auto inv_jj = (*divide)(_Ring_(1), v_jj);
		inv.data().Set(j, j, inv_jj);
	}
	
	return HBRS_MPL_FWD(inv);
}

/* namespace inverse_el */ }

template <typename Ring>
auto
inverse_impl_el_matrix::operator()(diagonal_matrix_without_zeros_on_main_diagonal<el_matrix<Ring>> const& x) const {
	typedef std::decay_t<Ring> _Ring_;
	return inverse_el::inverse(
		x,
		el_matrix<_Ring_>{0,0}
	);
}

template <typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
auto
inverse_impl_el_dist_matrix::operator()(diagonal_matrix_without_zeros_on_main_diagonal<el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping>> const& x) const {
	typedef std::decay_t<Ring> _Ring_;
	return inverse_el::inverse(
		x,
		el_dist_matrix<_Ring_, Columnwise, Rowwise, Wrapping> {x.data.data().Grid(), 0, 0}
	);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
#endif // !HBRS_MPL_FN_INVERSE_IMPL_ELEMENTAL_HPP
