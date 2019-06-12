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

#ifndef ELEMENTAL_FUSE_FN_ABSOLUTE_HPP
#define ELEMENTAL_FUSE_FN_ABSOLUTE_HPP

#include <elemental/config.hpp>
#include <hbrs/mpl/dt/el_matrix.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <boost/hana/tuple.hpp>
#include <type_traits>

ELEMENTAL_NAMESPACE_BEGIN
namespace mpl = hbrs::mpl;
namespace detail {

struct absolute_impl_matrix {
	template <
		typename Ring,
		typename std::enable_if_t<
			std::is_arithmetic< Ring >::value /* TODO: absolute() is not yet implemented for El::Complex<> */
		>* = nullptr
	>
	auto
	operator()(matrix<Ring> const& a) const {
		using namespace hbrs::mpl;
		
		matrix<std::remove_const_t<Ring>> b{ a.m(), a.n() };
		
		for(El::Int j=0; j < a.n(); ++j) {
			for(El::Int i=0; i < a.m(); ++i) {
				auto ix = make_matrix_index(i,j);
				(*at)(b, ix) = (*absolute)(at(a, ix));
			}
		}
		
		return b;
	}
};

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_ABSOLUTE_IMPLS boost::hana::make_tuple(                                                      \
		elemental::detail::absolute_impl_matrix{}                                                                      \
	)

#endif // !ELEMENTAL_FUSE_FN_ABSOLUTE_HPP
