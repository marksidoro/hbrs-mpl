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

#ifndef HBRS_MPL_FN_ANY_OF_IMPL_ELEMENTAL_HPP
#define HBRS_MPL_FN_ANY_OF_IMPL_ELEMENTAL_HPP

#include "../fwd/elemental.hpp"
#ifdef HBRS_MPL_ENABLE_ELEMENTAL

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/dt/el_matrix.hpp>
#include <hbrs/mpl/dt/el_vector.hpp>
#include <hbrs/mpl/dt/el_dist_matrix.hpp>
#include <hbrs/mpl/dt/el_dist_vector.hpp>
#include <hbrs/mpl/detail/mpi.hpp>
#include <vector>
#include <algorithm>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

template <
	typename Ring,
	typename UnaryPredicate
	//TODO: Add invokable check for UnaryPredicate?
>
static bool
any_of_el_matrix_impl(
	El::Matrix<Ring> const& a,
	UnaryPredicate && pred
) {
	for(El::Int j = 0; j < a.Width(); ++j) {
		for(El::Int i = 0; i < a.Height(); ++i) {
			if (evaluate(pred(a.Get(i,j)))) {
				return true;
			}
		}
	}
	return false;
}

template <
	typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping,
	typename UnaryPredicate
	//TODO: Add invokable check for UnaryPredicate?
>
static bool
any_of_el_dist_matrix_impl(
	El::DistMatrix<Ring, Columnwise, Rowwise, Wrapping> const& a,
	UnaryPredicate && pred
) {
	// Converting bool to int because std::vector<bool> does not provide pointer-access.
	int lcl = any_of_el_matrix_impl(a.LockedMatrix(), pred);
	std::vector<int> gbl(boost::numeric_cast<std::size_t>(mpi::size(a.Grid().Comm().comm)), false);
	mpi::allgather(&lcl, 1, gbl.data(), 1, a.Grid().Comm().comm);
	return std::any_of(gbl.begin(), gbl.end(), [](int x) { return x == true; });
}
	
template <
	typename Ring,
	typename UnaryPredicate
	//TODO: Add invokable check for UnaryPredicate?
>
bool
any_of_impl_el_matrix::operator()(
	el_matrix<Ring> const& a,
	UnaryPredicate && pred
) const {
	return any_of_el_matrix_impl(a.data(), HBRS_MPL_FWD(pred));
}

template <
	typename Ring,
	typename UnaryPredicate
	//TODO: Add invokable check for UnaryPredicate?
>
bool
any_of_impl_el_vector::operator()(
	el_column_vector<Ring> const& a,
	UnaryPredicate && pred
) const {
	return any_of_el_matrix_impl(a.data(), HBRS_MPL_FWD(pred));
}

template <
	typename Ring,
	typename UnaryPredicate
	//TODO: Add invokable check for UnaryPredicate?
>
bool
any_of_impl_el_vector::operator()(
	el_row_vector<Ring> const& a,
	UnaryPredicate && pred
) const {
	return any_of_el_matrix_impl(a.data(), HBRS_MPL_FWD(pred));
}

template <
	typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping,
	typename UnaryPredicate
	//TODO: Add invokable check for UnaryPredicate?
>
bool
any_of_impl_el_dist_matrix::operator()(
	el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping> const& a,
	UnaryPredicate && pred
) const {
	return any_of_el_dist_matrix_impl(a.data(), HBRS_MPL_FWD(pred));
}

template <
	typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping,
	typename UnaryPredicate
	//TODO: Add invokable check for UnaryPredicate?
>
bool
any_of_impl_el_dist_vector::operator()(
	el_dist_column_vector<Ring, Columnwise, Rowwise, Wrapping> const& a,
	UnaryPredicate && pred
) const {
	return any_of_el_dist_matrix_impl(a.data(), HBRS_MPL_FWD(pred));
}

template <
	typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping,
	typename UnaryPredicate
	//TODO: Add invokable check for UnaryPredicate?
>
bool
any_of_impl_el_dist_vector::operator()(
	el_dist_row_vector<Ring, Columnwise, Rowwise, Wrapping> const& a,
	UnaryPredicate && pred
) const {
	return any_of_el_dist_matrix_impl(a.data(), HBRS_MPL_FWD(pred));
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
#endif // !HBRS_MPL_FN_ANY_OF_IMPL_ELEMENTAL_HPP
