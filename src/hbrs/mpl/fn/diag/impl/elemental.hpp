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

#ifndef ELEMENTAL_FUSE_FN_DIAG_HPP
#define ELEMENTAL_FUSE_FN_DIAG_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>

#include <hbrs/mpl/dt/el_matrix/fwd.hpp>
#include <hbrs/mpl/dt/el_dist_matrix/fwd.hpp>
#include <hbrs/mpl/dt/el_vector.hpp>
#include <hbrs/mpl/dt/el_dist_vector.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <El.hpp>

#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

ELEMENTAL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace mpl = hbrs::mpl;

namespace detail {

struct diag_impl_matrix {
	template <typename Ring>
	auto
	operator()(matrix<Ring> const& m) const {
		using namespace hbrs::mpl;
		return make_column_vector(El::GetDiagonal(m.data()));
	}
};

//TODO: replace with a wrapper struct?!
struct diag_impl_dist_matrix {
	template <typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
	auto
	operator()(dist_matrix<Ring, Columnwise, Rowwise, Wrapping> const& m) const {
		using namespace hbrs::mpl;
		
		typedef decltype(m.data()) ElDistMatrix;
		typedef std::decay_t<ElDistMatrix> _ElDistMatrix_;
		typedef typename _ElDistMatrix_::diagType Diag;
		Diag v{m.data().Grid()};
		
		El::Int v_sz = El::Min(m.m(), m.n());
		v.Resize(v_sz, 1);
		
		for(El::Int i = 0; i < v_sz; ++i) {
			v.Set(i,0, m.data().Get(i,i)); //TODO: Replace global with local accesses?!
		}
		
		return make_dist_column_vector(std::move(v));
	}
};

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_DIAG_IMPLS boost::hana::make_tuple(                                                          \
		elemental::detail::diag_impl_matrix{},                                                                         \
		elemental::detail::diag_impl_dist_matrix{}                                                                     \
	)

#endif // !ELEMENTAL_FUSE_FN_DIAG_HPP
