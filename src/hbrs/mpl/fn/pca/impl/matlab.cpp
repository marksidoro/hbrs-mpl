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

#include <hbrs/mpl/fn/pca.hpp>
#include <hbrs/mpl/dt/smr.hpp>
#include <hbrs/mpl/dt/pca_result.hpp>
#include <hbrs/mpl/dt/ml_matrix.hpp>
#include <hbrs/mpl/dt/ml_vector.hpp>

extern "C" {
	#include <matlab/cxn/pca_level0.h>
	#include <matlab/cxn/pca_level1.h>
	#include <matlab/cxn/pca_level2.h>
}
#undef I /* I is defined by MATLAB Coder, but also used within Boost Unit Test Framework as a template parameter. */

MATLAB_NAMESPACE_BEGIN
namespace detail {

#define _DEF_PCA(lvl)                                                                                                  \
	mpl::pca_result<                                                                                                   \
		matlab::matrix<real_T> /* coeff */,                                                                            \
		matlab::matrix<real_T> /* score */,                                                                            \
		matlab::column_vector<real_T> /* latent*/,                                                                     \
		mpl::smr<matlab::matrix<real_T>, int> /* mu */                                                                 \
	>                                                                                                                  \
	pca_impl_level ## lvl::operator()(matlab::matrix<real_T> const& a, bool economy) const {                           \
		matlab::matrix<real_T> coeff, score;                                                                           \
		matlab::column_vector<real_T> latent;                                                                          \
		matlab::matrix<real_T> mu;                                                                                     \
		                                                                                                               \
		pca_level ## lvl(                                                                                              \
			&a.data(),                                                                                                 \
			economy,                                                                                                   \
			&coeff.data(),                                                                                             \
			&score.data(),                                                                                             \
			&latent.data(),                                                                                            \
			&mu.data()                                                                                                 \
		);                                                                                                             \
		                                                                                                               \
		return { coeff, score, latent, std::move(mu)[0] };                                                             \
	}

_DEF_PCA(0)
_DEF_PCA(1)
_DEF_PCA(2)
#undef _DEF_PCA

/* namespace detail */ }
MATLAB_NAMESPACE_END