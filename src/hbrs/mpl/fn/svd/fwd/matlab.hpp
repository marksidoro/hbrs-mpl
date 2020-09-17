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

#ifndef HBRS_MPL_FN_SVD_FWD_MATLAB_HPP
#define HBRS_MPL_FN_SVD_FWD_MATLAB_HPP

#include <hbrs/mpl/config.hpp>

#ifdef HBRS_MPL_ENABLE_MATLAB
	#include <hbrs/mpl/dt/decompose_mode/fwd.hpp>
	#include <hbrs/mpl/dt/svd_control/fwd.hpp>
	#include <hbrs/mpl/dt/svd_result/fwd.hpp>
	#include <hbrs/mpl/dt/ml_matrix/fwd.hpp>
#endif
#include <boost/hana/tuple.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

#ifdef HBRS_MPL_ENABLE_MATLAB

#define _DEC_SVD(lvl)                                                                                                  \
	struct svd_impl_level ## lvl ## _ml_matrix {                                                                       \
        HBRS_MPL_API                                                                                                   \
		svd_result<                                                                                                    \
			ml_matrix<real_T> /* U */,                                                                                 \
			ml_matrix<real_T> /* S */,                                                                                 \
			ml_matrix<real_T> /* V */                                                                                  \
		>                                                                                                              \
		operator()(                                                                                                    \
			ml_matrix<real_T> const& a,                                                                                \
			svd_control<decompose_mode> const& ctrl                                                                    \
		) const;                                                                                                       \
	};

_DEC_SVD(0)
_DEC_SVD(1)
#undef _DEC_SVD

#else
struct svd_impl_level0_ml_matrix{};
struct svd_impl_level1_ml_matrix{};
#endif

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_SVD_IMPLS_MATLAB boost::hana::make_tuple(                                                          \
		hbrs::mpl::detail::svd_impl_level0_ml_matrix{},                                                                \
		hbrs::mpl::detail::svd_impl_level1_ml_matrix{}                                                                 \
	)

#endif // !HBRS_MPL_FN_SVD_FWD_MATLAB_HPP
