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

#ifndef HBRS_MPL_FN_SORT_FWD_MATLAB_HPP
#define HBRS_MPL_FN_SORT_FWD_MATLAB_HPP

#include <hbrs/mpl/config.hpp>

#ifdef HBRS_MPL_ENABLE_MATLAB
	#include <hbrs/mpl/dt/ml_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/ml_vector/fwd.hpp>
	#include <hbrs/mpl/dt/eig_result/fwd.hpp>
	#include <functional>
#endif

#include <boost/hana/tuple.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

#ifdef HBRS_MPL_ENABLE_MATLAB

struct sort_impl_ml_eig_result {
	HBRS_MPL_API
	eig_result<ml_column_vector<real_T>, ml_matrix<real_T>>
	operator()(
		eig_result<ml_column_vector<real_T>, ml_matrix<real_T>> const&,
		std::function<bool(real_T, real_T)> const&
	) const;
	
	HBRS_MPL_API
	eig_result<ml_column_vector<creal_T>, ml_matrix<creal_T>>
	operator()(
		eig_result<ml_column_vector<creal_T>, ml_matrix<creal_T>> const&,
		std::function<bool(creal_T, creal_T)> const&
	) const;
};

#else
struct sort_impl_ml_eig_result {};
#endif

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_SORT_IMPLS_MATLAB boost::hana::make_tuple(                                                         \
		hbrs::mpl::detail::sort_impl_ml_eig_result{}                                                                   \
	)

#endif // !HBRS_MPL_FN_SORT_FWD_MATLAB_HPP
