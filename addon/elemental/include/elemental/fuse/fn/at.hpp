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

#ifndef ELEMENTAL_FUSE_FN_AT_HPP
#define ELEMENTAL_FUSE_FN_AT_HPP

#include <elemental/config.hpp>
#include <hbrs/mpl/preprocessor/core.hpp>
#include <hbrs/mpl/detail/function_object.hpp>

#include <hbrs/mpl/fwd/dt/matrix_index.hpp>
#include <elemental/fwd/dt/matrix.hpp>
#include <elemental/fwd/dt/vector.hpp>

#include <hbrs/mpl/dt/smr.hpp>
#include <El.hpp>

#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/assert.hpp>
#include <type_traits>

ELEMENTAL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace mpl = hbrs::mpl;

namespace detail {

struct at_impl_Matrix {
	template <
		typename Matrix,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::Matrix_tag >::value &&
			!std::is_const< std::remove_reference_t<Matrix&&> >::value
		>* = nullptr
	>
	decltype(auto)
	operator()(Matrix && m, mpl::matrix_index<El::Int, El::Int> const& i) const {
		BOOST_ASSERT(i.m() >= 0 && i.m() < m.Height());
		BOOST_ASSERT(i.n() >= 0 && i.n() < m.Width());
		return *HBRS_MPL_FWD(m).Buffer(i.m(), i.n());
	}
	
	template <
		typename Matrix,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::Matrix_tag >::value && 
			std::is_const< std::remove_reference_t<Matrix&&> >::value
		>* = nullptr
	>
	decltype(auto)
	operator()(Matrix && m, mpl::matrix_index<El::Int, El::Int> const& i) const {
		BOOST_ASSERT(i.m() >= 0 && i.m() < m.Height());
		BOOST_ASSERT(i.n() >= 0 && i.n() < m.Width());
		return *HBRS_MPL_FWD(m).LockedBuffer(i.m(), i.n());
	}
};

struct at_impl_Matrix_smr {
	template <
		typename Matrix,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::Matrix_tag >::value
		>* = nullptr
	>
	decltype(auto)
	operator()(Matrix && m, El::Int i) const {
		return mpl::smr<Matrix, El::Int>{HBRS_MPL_FWD(m), i};
	}
};

HBRS_MPL_DEF_FO_TRY_METHOD(at_impl_column_vector, column_vector_tag, at)
HBRS_MPL_DEF_FO_TRY_METHOD(at_impl_row_vector, row_vector_tag, at)

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_AT_IMPLS boost::hana::make_tuple(                                                            \
		elemental::detail::at_impl_column_vector{},                                                                    \
		elemental::detail::at_impl_row_vector{},                                                                       \
		elemental::detail::at_impl_Matrix{},                                                                           \
		elemental::detail::at_impl_Matrix_smr{}                                                                        \
	)

#endif // !ELEMENTAL_FUSE_FN_AT_HPP
