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

#ifndef MATLAB_FUSE_FN_BIDIAG_HPP
#define MATLAB_FUSE_FN_BIDIAG_HPP

#include <hbrs/mpl/preprocessor/core.hpp>
#include <hbrs/mpl/dt/decompose_mode.hpp>
#include <hbrs/mpl/dt/bidiag_result.hpp>
#include <matlab/dt/matrix.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

MATLAB_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace mpl = hbrs::mpl;
namespace detail {

#define _DEC_BIDIAG(lvl)                                                                                               \
	struct bidiag_impl_level ## lvl {                                                                                  \
		mpl::bidiag_result<                                                                                            \
			matlab::matrix<real_T> /* U */,                                                                            \
			matlab::matrix<real_T> /* B */,                                                                            \
			matlab::matrix<real_T> /* V */                                                                             \
		>                                                                                                              \
		operator()(matlab::matrix<real_T> const& a, mpl::decompose_mode mode) const;                                   \
	};

_DEC_BIDIAG(0)
#undef _DEC_BIDIAG

/* namespace detail */ }
MATLAB_NAMESPACE_END

#define MATLAB_FUSE_FN_BIDIAG_IMPLS boost::hana::make_tuple(                                                           \
		matlab::detail::bidiag_impl_level0{}                                                                           \
	)

#endif // !MATLAB_FUSE_FN_BIDIAG_HPP
