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

#ifndef HBRS_MPL_FN_EXPAND_FWD_MATLAB_HPP
#define HBRS_MPL_FN_EXPAND_FWD_MATLAB_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>

#ifdef HBRS_MPL_ENABLE_MATLAB
	#include <hbrs/mpl/dt/ml_matrix.hpp>
	#include <hbrs/mpl/dt/ml_vector.hpp>
#endif

#include <hbrs/mpl/dt/smr/fwd.hpp>
#include <hbrs/mpl/dt/matrix_size/fwd.hpp>

#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

#ifdef HBRS_MPL_ENABLE_MATLAB
//TODO: Join with expand_impl_smr_el_matrix!
struct expand_impl_smr_ml_matrix {
	template<
		typename Matrix,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, hbrs::mpl::ml_matrix_tag >::value
		>* = nullptr
	>
	auto
	operator()(
		smr<Matrix, int> const& a,
		matrix_size<int, int> const& sz
	) const;
};

//TODO: Join with expand_impl_el_row_vector!
struct expand_impl_ml_row_vector {
	template<typename Ring>
	auto
	operator()(
		row_vector<Ring> const& v,
		matrix_size<int, int> const& sz
	) const
};

#else
struct expand_impl_smr_ml_matrix {};
struct expand_impl_ml_row_vector {};
#endif

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_EXPAND_IMPLS_MATLAB boost::hana::make_tuple(                                                       \
		hbrs::mpl::detail::expand_impl_smr_ml_matrix{},                                                                \
		hbrs::mpl::detail::expand_impl_ml_row_vector{}                                                                 \
	)

#endif // !HBRS_MPL_FN_EXPAND_FWD_MATLAB_HPP
