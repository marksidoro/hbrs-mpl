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

#include "matlab.hpp"
#ifdef HBRS_MPL_ENABLE_MATLAB

#include <hbrs/mpl/dt/smr.hpp>
#include <hbrs/mpl/dt/pca_result.hpp>
#include <hbrs/mpl/dt/ml_matrix.hpp>
#include <hbrs/mpl/dt/ml_vector.hpp>

extern "C" {
	#include <hbrs/mpl/detail/matlab_cxn/impl/pca_level0.h>
	#include <hbrs/mpl/detail/matlab_cxn/impl/pca_level1.h>
	#include <hbrs/mpl/detail/matlab_cxn/impl/pca_level2.h>
}
#undef I /* I is defined by MATLAB Coder, but also used within Boost Unit Test Framework as a template parameter. */

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

#define _DEF_PCA(lvl)                                                                                                  \
	pca_result<                                                                                                        \
		ml_matrix<real_T>           /* coeff */,                                                                       \
		ml_matrix<real_T>           /* score */,                                                                       \
		ml_column_vector<real_T>    /* latent*/,                                                                       \
		smr<ml_matrix<real_T>, int> /* mu */                                                                           \
	>                                                                                                                  \
	pca_impl_level ## lvl ## _ml_matrix::operator()(ml_matrix<real_T> const& a, bool economy) const {                  \
		ml_matrix<real_T> coeff, score;                                                                                \
		ml_column_vector<real_T> latent;                                                                               \
		ml_matrix<real_T> mu;                                                                                          \
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
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_MATLAB
