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

#ifndef HBRS_MPL_FN_PLUS_IMPL_MATLAB_HPP
#define HBRS_MPL_FN_PLUS_IMPL_MATLAB_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/dt/smr/fwd.hpp>
#include <hbrs/mpl/dt/ml_matrix/fwd.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace mpl = hbrs::mpl;
namespace hana = boost::hana;
namespace detail {

struct plus_impl_matrix_scalar {
	hbrs::mpl::ml_matrix<real_T>
	operator()(hbrs::mpl::ml_matrix<real_T> const& a, real_T b) const;
};

struct plus_impl_matrix_matrix {
	hbrs::mpl::ml_matrix<real_T>
	operator()(hbrs::mpl::ml_matrix<real_T> const& a, hbrs::mpl::ml_matrix<real_T> const& b) const;
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_PLUS_IMPLS_MATLAB boost::hana::make_tuple(                                                             \
		matlab::detail::plus_impl_matrix_scalar{},                                                                     \
		matlab::detail::plus_impl_matrix_matrix{}                                                                      \
	)

#endif // !HBRS_MPL_FN_PLUS_IMPL_MATLAB_HPP
