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

#pragma once

#ifndef MATLAB_FUSE_FN_PLUS_HPP
#define MATLAB_FUSE_FN_PLUS_HPP

#include <matlab/config.hpp>
#include <hbrs/mpl/preprocessor/core.hpp>
#include <hbrs/mpl/fwd/dt/smr.hpp>
#include <matlab/fwd/dt/matrix.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

MATLAB_NAMESPACE_BEGIN
namespace mpl = hbrs::mpl;
namespace hana = boost::hana;
namespace detail {

struct plus_impl_matrix_scalar {
	matlab::matrix<real_T>
	operator()(matlab::matrix<real_T> const& a, real_T b) const;
};

struct plus_impl_matrix_matrix {
	matlab::matrix<real_T>
	operator()(matlab::matrix<real_T> const& a, matlab::matrix<real_T> const& b) const;
};

/* namespace detail */ }
MATLAB_NAMESPACE_END

#define MATLAB_FUSE_FN_PLUS_IMPLS boost::hana::make_tuple(                                                             \
		matlab::detail::plus_impl_matrix_scalar{},                                                                     \
		matlab::detail::plus_impl_matrix_matrix{}                                                                      \
	)

#endif // !MATLAB_FUSE_FN_PLUS_HPP
