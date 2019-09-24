/* Copyright (c) 2019 Abdullah Güntepe, <abdullah@guentepe.com>
 * Copyright (c) 2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_FN_SVD_IMPL_HBRS_MPL_HPP
#define HBRS_MPL_FN_SVD_IMPL_HBRS_MPL_HPP

#include "../fwd/hbrs_mpl.hpp"

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/dt/svd_control.hpp>
#include <hbrs/mpl/dt/decompose_mode.hpp>
#include <hbrs/mpl/dt/svd_result.hpp>
#include <hbrs/mpl/dt/rtsam.hpp>
#include <hbrs/mpl/dt/submatrix.hpp>
#include <hbrs/mpl/dt/rtsacv.hpp>
#include <hbrs/mpl/dt/rtsarv.hpp>
#include <hbrs/mpl/dt/range.hpp>
#include <hbrs/mpl/fn/bidiag.hpp>
#include <hbrs/mpl/fn/givens.hpp>
#include <hbrs/mpl/fn/almost_equal.hpp>
#include <hbrs/mpl/dt/almost_equal_control.hpp>
#include <hbrs/mpl/fn/select.hpp>
#include <hbrs/mpl/fn/multiply.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/greater_equal.hpp>
#include <hbrs/mpl/fn/less.hpp>
#include <hbrs/mpl/dt/exception.hpp>
#include <cmath>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

/*
 * Returns an array of lenght 2 that holds eigenvalues of 2x2 Matrix A.
 */
template<
	typename Ring,
	storage_order Order,
	typename Offset,
	typename Size
>
static std::array<Ring, 2>
eigenvalue_of_2x2_matrix_rtsam(
	submatrix<rtsam<Ring,Order>&, Offset,Size> const& a
) {
	if (a.at(make_matrix_index(0, 1)) == 0 && a.at(make_matrix_index(1, 0)) == 0) {
		return {1, 0};
	} else {
		auto T = 
			a.at(make_matrix_index(0, 0)) +
			a.at(make_matrix_index(1, 1));
		auto D =
			a.at(make_matrix_index(0, 0)) * a.at(make_matrix_index(1, 1)) -
			a.at(make_matrix_index(0, 1)) * a.at(make_matrix_index(1, 0));

		return {
			T / 2 + std::sqrt((T * T / 4 - D)),
			T / 2 - std::sqrt((T * T / 4 - D))
		};
	}
}

/*
 * Algorithm 8.6.1 (Golub-Kahan SVD Step) on page 491
 * Given a bidiagonal matrix B (which is of real numbers and of the
 * dimensions m-by-n) having no zeros on its diagonal or superdiagonal,
 * the following algorithm overwrites B with the bidiagonal matrix
 * B=U'BV where U and V are orthogonal and V is essentially the
 * orthogonal matrix that would be obtained by applying Algorithm
 * 8.3.2 to T=B'B.
 *
 * Contrary to the book this algorithm also computes U' and V. Given
 * matrices U' and V it overwrites them by applying the Givens
 * transformations calculated in this algorithm to them.
 *
 * In the Book B is actually B22 which is a submatrix of B. But here
 * we take B and the paramters p and q which are then used to create
 * the submatrix. Also p and q are necessary for the calculation of U
 * and V.
 */
template<typename B_, typename U_, typename V_>
static void
svd_step_rtsam(B_& B, std::size_t p, std::size_t q, U_& U, V_& V) {
	// The range for B22 rows and columns
	range<std::size_t,std::size_t> pq = {
		p,
		(*n)(size(B)) - 1 - q
	};
	
	auto B22 = (*select)(B, std::make_pair(pq, pq));

	/* Let mu be the eigenvalue of the trailing 2-by-2 submatrix of
	 * T=B'B that is closer to tnn.
	 *
	 * Calculate mu.
	 */
	auto T = (*multiply)(transpose(B22), B22);
	range<std::size_t,std::size_t> T22 = {
		(*m)(size(T)) - 2,
		(*m)(size(T)) - 1
	};
	auto l = eigenvalue_of_2x2_matrix_rtsam((*select)(T, std::make_pair(T22, T22)));
	auto tnn = T[(*m)(size(T))-1][(*m)(size(T))-1];
	auto mu = std::abs(l.at(0) - tnn) < std::abs(l.at(1) - tnn) ? l.at(0) : l.at(1);

	auto y = T[0][0] - mu;
	auto z = T[0][1];
	
	for (std::size_t k = 0; k < (*n)(size(B22))-1; ++k) {
		{
			// Determine c = cos(theta) and s = sin(theta) such that
			auto theta = (*givens)(y, z);
			B22 = (*multiply)(B22, G(  k,   k + 1, theta));
			V   = (*multiply)(V,   G(p+k, p+k + 1, theta));

			y = B22[k  ][k];
			z = B22[k+1][k];
		}
		{
			// Determine c = cos(theta) and s = sin(theta) such that
			auto theta = (*givens)(y, z);
			B22 = (*multiply)(G(k, k + 1, theta), B22);
			U   = (*multiply)(U, G(p+k, p+k + 1, theta));
			if (k + 1 < (*n)(size(B22))-1) {
				y = B22[k][k+1];
				z = B22[k][k+2];
			}
		}
	}
}

/*
 * Algorithm 8.6.2 (The SVD Algorithm) on page 492
 * Given matrix A (which is real and m-by-n) (m>=n) the following
 * algorithm computes the SVD, where U (which is real and m-by-m) is
 * orthogonal and V (which is real and n-by-n) is orthogonal.
 *
 * Contrary to the book this algorithm also computes U' and V. Taking
 * matrices U' and V from the bidiagonalization it overwrites them by
 * applying the Givens transformations calculated in this algorithm to
 * them.
 *
 * Instead of overwriting A this algorithm stores A, U' and V and
 * returns them in the Struct SVDResult.
 */

template<
	typename Ring,
	storage_order Order,
	typename std::enable_if_t<
		// TODO: Drop this restriction to double once almost_equal has been implemented for other arithmetic types
		std::is_same_v< std::decay_t<Ring>, double >
	>*
>
auto
svd_impl_rtsam::operator()(
	rtsam<Ring,Order> const& a,
	svd_control<decompose_mode> const& ctrl
) const {
	if (ctrl.decompose_mode() != decompose_mode::complete) {
		BOOST_THROW_EXCEPTION(not_supported_exception{} << errinfo_decompose_mode{ctrl.decompose_mode()});
	}
	//TODO: Implement other decomposition modes
	BOOST_ASSERT(ctrl.decompose_mode() == decompose_mode::complete);
	
	auto a_sz = (*size)(a);
	auto a_m = (*m)(a_sz);
	auto a_n = (*n)(a_sz);
	
	if ((*less)(a_m, a_n)) {
		BOOST_THROW_EXCEPTION(incompatible_matrix_exception{} << errinfo_matrix_size{a_sz});
	}
	
	//TODO: Implement support for a_m < a_n
	BOOST_ASSERT((*greater_equal)(a_m, a_n));

	//TODO: Make almost_equal_control customizable
	almost_equal_control<int,int> aeq_ctrl{147,2};
	
	//Use Algorithm 5.4.2 to compute the bidiagonalization.
	auto UBV = (*bidiag)(a, make_bidiag_control(ctrl.decompose_mode()));
	auto U = std::move(UBV.u());
	auto B = std::move(UBV.b());
	auto V = std::move(UBV.v());

	auto n_ = a_n;

	std::size_t q = 0;
	std::size_t p = 0;

	while (q != n_) {
		for (std::size_t i = 0; i < n_ - 1; ++i) {
			if ((*almost_equal)(0., B[i][i+1], aeq_ctrl)) {
				B[i][i+1] = 0;
			}
		}

		/*
		 * Find the largest q and the smallest p such that if
		 *     -----------------
		 *     | B11   0    0  |    p
		 *     |               |
		 * B = |  0   B22   0  |  n-p-q
		 *     |               |
		 *     |  0    0   B33 |    q
		 *     -----------------
		 *        p  n-p-q  q
		 * then B33 is diagonal and B22 has a nonzero superdiagonal.
		 */
		
		{ // First find q
			for (q = 0; q < n_; ++q) {
				if (q == n_-1) {
					q = n_;
					break;
				} else if (B[n_-1 - q - 1][n_-1 - q] != 0) {
					break;
				}
			}
		}
		
		if (q < n_) {
			/*
			 * Second find p
			 * In the book this part is before the if condition. We
			 * moved it inside the if condition for optimization.
			 */
			{
				for (p = n_-1 - q; p >= 1; --p) {
					if (B[p-1][p] == 0) {
						break;
					} else if (p == 1) {
						p = 0;
						break;
					}
				}
			}
			
			/*
			 * if any diagonal entry in B22 is zero, then zero the
			 * superdiagonal entry in the same row.
			 */
			bool zero_found = false; // Turns true once a zero is found in the bidiagonal of B22
			for (std::size_t i = p; i < n_ - q; ++i) {
				if (B[i][i] == 0) {
					zero_found = true;
					if (i < n_-1 - q) {
						for (std::size_t j = i + 1; j < n_ - q; ++j) {
							auto theta = (*givens)(-B[j][j], B[i][j]);
							B = (*multiply)(G(i, j, theta), B);
							U = (*multiply)(U, G(i, j, theta));
						}
					} else {
						for (std::size_t j = n_-1 - q - 1; j >= p; --j) {
							auto theta = (*givens)(B[j][j], B[j][n_-1 - q]);
							B = (*multiply)(B, G(j, n_-1 - q, theta));
							V = (*multiply)(V, G(j, n_-1 - q, theta));
						}
					}
				}
			}
			
			if (!zero_found) {
				svd_step_rtsam(B, p, q, U, V); // Apply Algorithm 8.6.1 to B22
				/* In the book there is another line here that looks
				 * like this:
				 * B = diag(Ip,U,Iq+m-n)' B diag(Ip,V,Iq)
				 * But that line is actually not part of the algorithm.
				 */
			}
		}
	}
	
	return make_svd_result(U, B, V);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FN_SVD_IMPL_HBRS_MPL_HPP
