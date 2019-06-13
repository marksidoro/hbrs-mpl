/* Copyright (c) 2016-2018 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_FN_PCA_IMPL_MATLAB_HPP
#define HBRS_MPL_FN_PCA_IMPL_MATLAB_HPP

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/dt/pca_result.hpp>
#include <hbrs/mpl/dt/smr.hpp>
#include <hbrs/mpl/dt/ml_matrix.hpp>
#include <hbrs/mpl/dt/ml_vector.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace mpl = hbrs::mpl;

namespace detail {

#define _DEC_PCA(lvl)                                                                                                  \
	struct pca_impl_level ## lvl {                                                                                     \
		mpl::pca_result<                                                                                               \
			hbrs::mpl::ml_matrix<real_T> /* coeff */,                                                                        \
			hbrs::mpl::ml_matrix<real_T> /* score */,                                                                        \
			hbrs::mpl::ml_column_vector<real_T> /* latent*/,                                                                 \
			mpl::smr<hbrs::mpl::ml_matrix<real_T>, int> /* mu */                                                             \
		>                                                                                                              \
		operator()(hbrs::mpl::ml_matrix<real_T> const& a, bool economy) const;                                               \
	};

_DEC_PCA(0)
_DEC_PCA(1)
_DEC_PCA(2)
#undef _DEC_PCA

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_PCA_IMPLS_MATLAB boost::hana::make_tuple(                                                              \
		matlab::detail::pca_impl_level0{},                                                                             \
		matlab::detail::pca_impl_level1{},                                                                             \
		matlab::detail::pca_impl_level2{}                                                                              \
	)

#endif // !HBRS_MPL_FN_PCA_IMPL_MATLAB_HPP
