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

#ifndef HBRS_MPL_FN_MLDIVIDE_IMPL_ELEMENTAL_HPP
#define HBRS_MPL_FN_MLDIVIDE_IMPL_ELEMENTAL_HPP

#include "../fwd/elemental.hpp"
#ifdef HBRS_MPL_ENABLE_ELEMENTAL

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>

#include <hbrs/mpl/dt/el_matrix.hpp>
#include <hbrs/mpl/dt/el_dist_matrix.hpp>
#include <hbrs/mpl/dt/el_vector.hpp>
#include <hbrs/mpl/dt/el_dist_vector.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/equal.hpp>
#include <hbrs/mpl/fn/less.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template <typename A, typename X, typename B>
static void
mldivide_el_matrix_el_vector_impl(A const& a, X & x, B const& b) {
	auto a_sz = (*size)(a);
	auto a_m = (*m)(a_sz);
	auto a_n = (*n)(a_sz);
	
	// TODO: Choose appropriate solver as e.g. MATLAB does in https://de.mathworks.com/help/matlab/ref/mldivide.html
	/* NOTE:
	 * Let A be m*n and B be m.
	 *
	 * If m==n, then mldivide(A,B) from MATLAB Coder [1] uses lusolve(A,B) else qrsolve(A,B).
	 * If A's rank is less than the number of columns in A (a.k.a. A is under underdetermined; a.k.a. infinite solutions
	 * exist to Ax=b), then x = mldivide(A,B) is not necessarily the minimum norm solution. [4]
	 * mldivide(A,B) computes one solution that minimizes ||Ax-b||, but this solution typically does not minimize ||x||.
	 * The solution computed by lsqminnorm minimizes not only norm(A*x-b), but also norm(x). [5]
	 * 
	 * LeastSquares() from Elemental [2] uses QR() and qr::SolveAfter if m>=n, else LQ() and lq::SolveAfter() [3].
	 *
	 * References:
	 * [1] ./toolbox/eml/lib/matlab/ops/mldivide.m
	 * [2] src/lapack_like/euclidean_min/LeastSquares.cpp
	 * [3] https://www.netlib.org/lapack/lug/node41.html
	 * [4] https://de.mathworks.com/help/matlab/ref/mldivide.html
	 * [5] https://de.mathworks.com/help/matlab/ref/lsqminnorm.html
	 */
	// TODO: Either change MATLAB code or Elemental code to get equal results.
	
	if ((*equal)(a_m, a_n)) {
		// a is square matrix
		x = b;
		El::LinearSolve(a.data(), x.data());
		BOOST_ASSERT(x.data().Width() == 1);
	} else {
		// if a_m < a_n then a is underdetermined
		// if a_m > a_n then a is overdetermined
		El::LeastSquares(El::NORMAL, a.data(), b.data(), x.data());
		BOOST_ASSERT(x.data().Width() == 1);
	}
}

template <typename Ring>
auto
mldivide_impl_el_matrix_el_vector::operator()(
	el_matrix<Ring> const& a,
	el_column_vector<Ring> const& b
) const {
	el_column_vector<Ring> x{0};
	mldivide_el_matrix_el_vector_impl(a, x, b);
	return x;
}

template <
	typename Ring, El::Dist ColumnwiseL, El::Dist RowwiseL, El::DistWrap Wrapping,
	/*          */ El::Dist ColumnwiseR, El::Dist RowwiseR
>
auto
mldivide_impl_el_dist_matrix_el_dist_vector::operator()(
	el_dist_matrix<Ring, ColumnwiseL, RowwiseL, Wrapping> const& a,
	el_dist_column_vector<Ring, ColumnwiseR, RowwiseR, Wrapping> const& b
) const {
	el_dist_column_vector<Ring, ColumnwiseR, RowwiseR, Wrapping> x{a.data().Grid(), 0};
	mldivide_el_matrix_el_vector_impl(a, x, b);
	return x;
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
#endif // !HBRS_MPL_FN_MLDIVIDE_IMPL_ELEMENTAL_HPP
