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

#ifndef ELEMENTAL_FUSE_FN_TRANSPOSE_HPP
#define ELEMENTAL_FUSE_FN_TRANSPOSE_HPP

#include <elemental/config.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/dt/el_matrix.hpp>
#include <hbrs/mpl/dt/el_dist_matrix.hpp>
#include <El.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

ELEMENTAL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

struct transpose_impl_matrix {
	template<
		typename Matrix,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Matrix>, matrix_tag >::value 
		>* = nullptr
	>
	auto
	operator()(Matrix && m) const {
		typedef decltype(m.at({0,0})) Ring;
		typedef std::decay_t<Ring> _Ring_;
		
		El::Matrix<_Ring_> b;
		El::Transpose(HBRS_MPL_FWD(m).data(), b);
		return make_matrix(std::move(b));
	}
};

struct transpose_impl_dist_matrix {
	template<
		typename DistMatrix,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<DistMatrix>, dist_matrix_tag >::value 
		>* = nullptr
	>
	auto
	operator()(DistMatrix && m) const {
		typedef decltype(m.data()) ElDistMatrix;
		typedef std::decay_t<ElDistMatrix> _ElDistMatrix_;
		typedef typename _ElDistMatrix_::transType Transposed;
		
		Transposed b{m.data().Grid()};
		El::Transpose(HBRS_MPL_FWD(m).data(), b);
		return make_dist_matrix(std::move(b));
	}
};

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_TRANSPOSE_IMPLS boost::hana::make_tuple(                                                     \
		elemental::detail::transpose_impl_matrix{},                                                                    \
		elemental::detail::transpose_impl_dist_matrix{}                                                                \
	)

#endif // !ELEMENTAL_FUSE_FN_TRANSPOSE_HPP
