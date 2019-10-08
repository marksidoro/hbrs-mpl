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

#ifndef HBRS_MPL_FN_EIG_IMPL_ELEMENTAL_HPP
#define HBRS_MPL_FN_EIG_IMPL_ELEMENTAL_HPP

#include "../fwd/elemental.hpp"
#ifdef HBRS_MPL_ENABLE_ELEMENTAL

#include <hbrs/mpl/core/preprocessor.hpp>

#include <hbrs/mpl/dt/eig_control.hpp>
#include <hbrs/mpl/dt/eig_result.hpp>
#include <hbrs/mpl/dt/el_matrix.hpp>
#include <hbrs/mpl/dt/el_vector.hpp>
#include <hbrs/mpl/dt/el_dist_matrix.hpp>
#include <hbrs/mpl/dt/el_dist_vector.hpp>
#include <hbrs/mpl/dt/exception.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/not_equal.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

/* Code of El::Eig() [1] is similar to:
 * ( Matrix<Complex<Real>>& A,
 *   Matrix<Complex<Real>>& w,
 *   Matrix<Complex<Real>>& X )
 * {
 *     // A is input matrix
 *     // w contains eigenvalues of A
 *     // X contains eigenvectors of A.
 *     typedef Complex<Real> C;
 * 
 *     Schur( A, w, X );
 * 
 *     Matrix<C> R;
 *     TriangEig( A, R );
 * 
 *     Trmm( RIGHT, UPPER, NORMAL, NON_UNIT, C(1), R, X );
 * }
 * 
 * Schur() computes the Schur decomposition, using LAPACK's Hessenberg QR
 * algorithm (with Aggressive Early Deflation) for local matrices, and if
 * support for ScaLAPACK was detected during configuration, ScaLAPACK's
 * Hessenberg QR algorithm (without deflation) [2].
 * 
 * TriangEig() computes eigenvectors of a triangular matrix [3] 
 * and is implemented in [4].
 * 
 * xTRMM() from BLAS performs " :math:`C := \alpha \mbox{op}(A) B` or
 * :math:`C := \alpha B \mbox{op}(A)`, depending upon whether `side` was 
 * chosen as 'L' or 'R', respectively. Whether :math:`A` is treated as lower 
 * or upper triangular is determined by whether `uplo` is 'L' or 'U' (setting
 * `unit` equal to 'U' treats :math:`A` as unit diagonal, otherwise it should
 * be set to 'N'). :math:`\mbox{op}` is determined in the same manner as in 
 * ``blas::Trmv``." [5]
 * 
 * NOTE: MATLAB Coder uses LAPACK's xGEEV for eig() and thus
 *       will likely produce different decompositions [6].
 * 
 * Ref.:
 * [1] Elemental/src/lapack_like/spectral/Eig.cpp 
 * [2] elemental-web/source/doc-dev/lapack_like/spectral/Schur.rst
 * [3] Elemental/include/El/lapack_like/spectral.hpp
 * [4] Elemental/src/lapack_like/spectral/TriangEig.cpp
 * [5] elemental-web/source/doc-dev/core/imports/blas.rst
 * [6] src/hbrs/mpl/detail/matlab_cxn/impl/eig_level0.m
 */

template <
	typename Ring,
	typename std::enable_if_t<
		!std::is_reference_v<Ring> &&
		!std::is_const_v<Ring> &&
		std::is_arithmetic_v<Ring>
	>*
>
decltype(auto)
eig_impl_el_matrix::operator()(
	el_matrix<Ring> const& a,
	eig_control<> const& ctrl
) const {
	auto a_sz = (*size)(a);
	auto a_m = (*m)(a_sz);
	auto a_n = (*n)(a_sz);
	
	if ((*not_equal)(a_m, a_n)) {
		BOOST_THROW_EXCEPTION(incompatible_matrix_exception{} << errinfo_el_matrix_size{a_sz});
	}
	
	auto x = a;
	el_column_vector<El::Complex<El::Base<Ring>>> w{0};
	el_matrix<El::Complex<El::Base<Ring>>> X{0, 0};
	El::Eig(x.data(), w.data(), X.data());
	return make_eig_result(w, X);
}

template<
	typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping,
	typename std::enable_if_t<
		!std::is_reference_v<Ring> &&
		!std::is_const_v<Ring> &&
		std::is_arithmetic_v<Ring>
	>*
>
decltype(auto)
eig_impl_el_dist_matrix::operator()(
	el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping> const& a,
	eig_control<> const& ctrl
) const {
	auto a_sz = (*size)(a);
	auto a_m = (*m)(a_sz);
	auto a_n = (*n)(a_sz);
	
	if ((*not_equal)(a_m, a_n)) {
		BOOST_THROW_EXCEPTION(incompatible_matrix_exception{} << errinfo_el_matrix_size{a_sz});
	}
	
	auto x = a;
	el_dist_column_vector<El::Complex<El::Base<Ring>>> w{a.data().Grid(), 1};
	el_dist_matrix<El::Complex<El::Base<Ring>>> X{a.data().Grid(), 1, 1};
	
	El::Eig(x.data(), w.data(), X.data());
	return make_eig_result(w, X);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
#endif // !HBRS_MPL_FN_EIG_IMPL_ELEMENTAL_HPP
