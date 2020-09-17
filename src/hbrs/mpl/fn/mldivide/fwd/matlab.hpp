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

#ifndef HBRS_MPL_FN_MLDIVIDE_FWD_MATLAB_HPP
#define HBRS_MPL_FN_MLDIVIDE_FWD_MATLAB_HPP

#include <hbrs/mpl/config.hpp>

#ifdef HBRS_MPL_ENABLE_MATLAB
	#include <hbrs/mpl/dt/ml_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/ml_vector/fwd.hpp>
#endif

#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

#ifdef HBRS_MPL_ENABLE_MATLAB
struct mldivide_impl_ml_matrix_ml_vector {
	HBRS_MPL_API
	ml_column_vector<real_T>
	operator()(ml_matrix<real_T> const& a, ml_column_vector<real_T> const& b) const;
	
	HBRS_MPL_API
	ml_column_vector<creal_T>
	operator()(ml_matrix<creal_T> const& a, ml_column_vector<creal_T> const& b) const;
};
#else
struct mldivide_impl_ml_matrix_ml_vector{};
#endif

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_MLDIVIDE_IMPLS_MATLAB boost::hana::make_tuple(                                                     \
		hbrs::mpl::detail::mldivide_impl_ml_matrix_ml_vector{}                                                         \
	)

#endif // !HBRS_MPL_FN_MLDIVIDE_FWD_MATLAB_HPP
