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

#include "../fwd/hbrs_mpl.hpp"

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/dt/bidiag_control.hpp>
#include <hbrs/mpl/dt/bidiag_result.hpp>
#include <hbrs/mpl/dt/decompose_mode.hpp>
#include <hbrs/mpl/dt/rtsacv.hpp>
#include <hbrs/mpl/dt/rtsarv.hpp>
#include <hbrs/mpl/dt/rtsam.hpp>
#include <hbrs/mpl/dt/range.hpp>
#include <hbrs/mpl/fn/house.hpp>
#include <hbrs/mpl/fn/minus.hpp>
#include <hbrs/mpl/fn/multiply.hpp>
#include <hbrs/mpl/fn/transpose.hpp>
#include <hbrs/mpl/fn/select.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/greater_equal.hpp>
#include <cmath>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

/*
 * Algorithm 5.4.2 (Householder Bidiagonalization) on page 284
 * Given the real m-by-n matrix A with m>=n, the following algorithm
 * creates B,U' and V with U'AV=B where B is upper bidiagonal and U'
 * and V are products of householder matrices.
 *
 * In comparison with the book:
 *      In the book the Algorithm directly overwrites A. Instead we
 *      make a copy of A, and call it B, and we overwrite B. We don't
 *      overwrite A. And instead of storing U' and V together with B
 *      inside A, we return a type that contains U', B and V called
 *      BidiagResult.
 */
template<
	typename Ring,
	storage_order Order
>
auto
bidiag_impl_rtsam::operator()(rtsam<Ring,Order> const& x, bidiag_control<decompose_mode> const& ctrl) const {
	//TODO: Implement other decomposition modes!
	BOOST_ASSERT(ctrl.decompose_mode() == decompose_mode::complete);
	
	auto x_sz = (*size)(x);
	auto x_m = (*m)(x_sz);
	auto x_n = (*n)(x_sz);
	
	BOOST_ASSERT((*greater_equal)(x_m, x_n));

	// returns a square Identity Matrix with size amount of rows and columns
	auto make_identity = [](std::size_t size) {
		typedef std::decay_t<Ring> _Ring_;
		rtsam<_Ring_,Order> id_ {size, size};
		for (std::size_t i = 0; i < size; ++i) {
			id_.at(make_matrix_index(i, i)) = _Ring_(1);
		}
		return id_;
	};
	
	auto U = make_identity(x_m);
	auto A = x;
	auto V = make_identity(x_n);

	for (std::size_t j = 0; j < x_n - 1; ++j) {
		range<std::size_t,std::size_t> jm  {j    , x_m-1};
		range<std::size_t,std::size_t> jn  {j    , x_n-1};
		range<std::size_t,std::size_t> j1n {j + 1, x_n-1};

		//Use Algorithm 5.1.1 to compute the householder vector.
		auto h = (*house)(select(A, std::make_pair(jm, j)));
		decltype(auto) ni = h.ni();
		decltype(auto) beta = h.beta();

		/*
			* Here the first commented out line is the one on page 285
			* in Algorithm 5.4.2. But as explained on page 236 in
			* Chapter 5.1.4 it should never be implemented that way.
			* So we implemented the way it is suggested on that page.
			*/

		/* Ajmjn is equivalent to A(j:x_m,j:x_n) in the book */
		auto Ajmjn = (*select)(A, std::make_pair(jm, jn));
		/* Ajmjn = (*multiply)(minus(make_identity((*minus)(x_m, j)), multiply(multiply(beta,ni), transpose(ni))), Ajmjn); // mathematical notation */
		Ajmjn = (*minus)(Ajmjn, multiply(multiply(beta, ni), multiply(transpose(ni), Ajmjn)));
		/*
			* In the book here the householder vector would be saved
			* inside of A. But since we compute and return U we don't
			* do that.
			*/

		auto Ui = make_identity(x_m);
		(*select)(Ui, std::make_pair(jm,jm)) = (*minus)(make_identity((*minus)(x_m, j)), multiply(beta, multiply(ni, transpose(ni))));
		U = (*multiply)(Ui, U);

		if (j + 1 <= x_n - 2) {
			auto h = (*house)(
				transpose(
					select(A, std::make_pair(j, j1n))
				)
			);
			decltype(auto) ni = h.ni();
			decltype(auto) beta = h.beta();

			auto Ajmj1n = (*select)(A, std::make_pair(jm, j1n)); // equivalent to A(j:x_m,j+1:x_n)
			/* Ajmj1n = (*multiply)(Ajmj1n, minus((make_identity((*minus)(minus(x_n,1),j)), multiply(multiply(beta,ni),transpose(ni))))); // mathematical notation */
			Ajmj1n = (*minus)(Ajmj1n, multiply(multiply(Ajmj1n, ni), transpose(multiply(beta, ni))));
			/*
				* In the book here the householder vector would be saved
				* inside of A. But since we compute and return V we don't
				* do that.
				*/

			auto Vjmj1n = (*select)(V, std::make_pair(jn, j1n));
			Vjmj1n = (*minus)(Vjmj1n, multiply(multiply(Vjmj1n,ni), transpose(multiply(beta,ni))));
		}
	}
	
	return make_bidiag_result((*transpose)(U), A, V);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END
