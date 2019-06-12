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

#ifndef MATLAB_FUSE_FN_COLUMNS_HPP
#define MATLAB_FUSE_FN_COLUMNS_HPP

#include <hbrs/mpl/preprocessor/core.hpp>
#include <matlab/fwd/dt/matrix.hpp>
#include <hbrs/mpl/dt/smcs.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

MATLAB_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

struct columns_impl_matrix {
	mpl::smcs<matlab::matrix<real_T>>
	operator()(matlab::matrix<real_T> a) const;
};

/* namespace detail */ }
MATLAB_NAMESPACE_END

#define MATLAB_FUSE_FN_COLUMNS_IMPLS boost::hana::make_tuple(                                                          \
		matlab::detail::columns_impl_matrix{}                                                                          \
	)

#endif // !MATLAB_FUSE_FN_COLUMNS_HPP
