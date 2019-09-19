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
#include <hbrs/mpl/dt/bidiag_result.hpp>
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
decltype(auto)
bidiag_impl_householder::operator()(rtsam<Ring,Order> const& A, int econ) {
	BOOST_ASSERT(m(size(A)) >= n(size(A)));

	// Copy A to result.b()
	auto result {make_bidiag_result(identity<Ring,Order>(m(size(A))), A, identity<Ring,Order>(n(size(A))))};

	{ /* This block is here so the reference A in the next line can be
		created for readability */
		auto& U {result.u()};
		auto& A {result.b()};
		auto& V {result.v()};

		auto const m {size(A).m()};
		auto const n {size(A).n()};

		for (std::size_t j {0}; j < n - 1; ++j) {
			range<std::size_t,std::size_t> const jm     {j             , m-1};
			range<std::size_t,std::size_t> const jn     {j	           , n-1};
			range<std::size_t,std::size_t> const j1n    {j + 1         , n-1};

			//Use Algorithm 5.1.1 to compute the householder vector.
			auto const h {house(select(A, std::make_pair(jm, j)))};
			auto& ni {h.ni()};
			auto& beta {h.beta()};

			/*
			 * Here the first commented out line is the one on page 285
			 * in Algorithm 5.4.2. But as explained on page 236 in
			 * Chapter 5.1.4 it should never be implemented that way.
			 * So we implemented the way it is suggested on that page.
			 */

			/* Ajmjn is equivalent to A(j:m,j:n) in the book */
			auto Ajmjn {select(A, std::make_pair(jm, jn))};
			/* Ajmjn = (identity<Ring,Order>(m - j) - beta * ni * transpose(ni)) * Ajmjn; // mathematical notation */
			Ajmjn = Ajmjn - (beta * ni) * (transpose(ni) * Ajmjn);
			/*
			 * In the book here the householder vector would be saved
			 * inside of A. But since we compute and return U we don't
			 * do that.
			 */

			auto Ui {identity<Ring,Order>(m)};
			select(Ui, std::make_pair(jm,jm)) = identity<Ring,Order>(m - j) - beta * (ni * transpose(ni));
			U = Ui * U;

			if (j + 1 <= n - 2) {
				auto const h {house(transpose(select(A, std::make_pair(j, j1n))))};
				auto& ni {h.ni()};
				auto& beta {h.beta()};

				auto Ajmj1n {select(A, std::make_pair(jm, j1n))}; // equivalent to A(j:m,j+1:n)
				/* Ajmj1n = Ajmj1n * (identity<Ring,Order>(n-1 - j) - beta * ni * transpose(ni)); // mathematical notation */
				Ajmj1n = Ajmj1n - (Ajmj1n * ni) * transpose(beta * ni);
				/*
				 * In the book here the householder vector would be saved
				 * inside of A. But since we compute and return V we don't
				 * do that.
				 */

				auto Vjmj1n {select(V, std::make_pair(jn, j1n))};
				Vjmj1n = Vjmj1n - (Vjmj1n * ni) * transpose(beta * ni);
			}
		}
		U = transpose(U);
	}
	return result;
}
// returns a square Identity Matrix with size amount of rows and columns
template<
	typename Ring,
	storage_order Order
>
rtsam<Ring,Order>
bidiag_impl_householder::identity(std::size_t const size) {
	typedef std::decay_t<Ring> _Ring_;
	rtsam<_Ring_,Order> result {size, size};
	for (std::size_t i {0}; i < size; ++i) {
		for (std::size_t j {0}; j < size; ++j) {
			result.at(make_matrix_index(i, j)) = 0;
		}
		result.at(make_matrix_index(i, i)) = 1;
	}
	return result;
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END