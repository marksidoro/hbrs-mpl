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

#ifndef ELEMENTAL_FUSE_FN_DIAG_HPP
#define ELEMENTAL_FUSE_FN_DIAG_HPP

#include <elemental/config.hpp>
#include <hbrs/mpl/preprocessor/core.hpp>

#include <elemental/fwd/dt/matrix.hpp>
#include <elemental/fwd/dt/dist_matrix.hpp>
#include <elemental/dt/vector.hpp>
#include <elemental/dt/dist_vector.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <El.hpp>

#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

ELEMENTAL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace mpl = hbrs::mpl;

namespace detail {

struct diag_impl_Matrix {
	template <typename Ring>
	auto
	operator()(El::Matrix<Ring> const& m) const {
		using namespace hbrs::mpl;
		return make_column_vector(El::GetDiagonal(m));
	}
};

//TODO: replace this hack!
struct diag_impl_DistMatrix {
	template <
		typename DistMatrix,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<DistMatrix>, hana::ext::El::DistMatrix_tag >::value
		>* = nullptr
	>
	auto
	operator()(DistMatrix const& m) const {
		using namespace hbrs::mpl;
		El::Int v_sz = El::Min(m.Height(), m.Width());
		typedef typename std::decay_t<DistMatrix>::diagType Diag;
		Diag v{m.Grid()};
		v.Resize(v_sz, 1);
		
		for(El::Int i = 0; i < v_sz; ++i) {
			v.Set(i,0, m.Get(i,i)); //TODO: Replace global with local accesses?!
		}
		
		return dist_column_vector<Diag>{v};
	}
};

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_DIAG_IMPLS boost::hana::make_tuple(                                                          \
		elemental::detail::diag_impl_Matrix{},                                                                         \
		elemental::detail::diag_impl_DistMatrix{}                                                                      \
	)

#endif // !ELEMENTAL_FUSE_FN_DIAG_HPP
