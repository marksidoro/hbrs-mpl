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

#ifndef HBRS_MPL_FN_EXPAND_IMPL_HBRS_MPL_HPP
#define HBRS_MPL_FN_EXPAND_IMPL_HBRS_MPL_HPP

#include "../fwd/hbrs_mpl.hpp"

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/dt/srv.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>
#include <hbrs/mpl/dt/rtsam.hpp>
#include <hbrs/mpl/dt/exception.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/at.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template <
	typename Ring,
	typename std::enable_if_t<
		std::is_arithmetic<std::decay_t<Ring>>::value
	>*
>
auto
expand_impl_srv::operator()(srv<std::vector<Ring>> const& v, matrix_size<std::size_t, std::size_t> sz) const {
	typedef std::decay_t<Ring> _Ring_;
	auto v_sz = (*size)(v);
	auto a_m = (*m)(sz);
	auto a_n = (*n)(sz);
	
	if (((a_n % v_sz) != 0) || (a_m == 0)) {
		BOOST_THROW_EXCEPTION((
			incompatible_matrix_vector_exception{}
			<< errinfo_vector_size{v_sz}
			<< errinfo_matrix_size{sz}
		));
	}
	
	bool vert_expand = a_m > 1;
	bool horz_expand = a_n != v_sz;
	
	rtsam<_Ring_, storage_order::row_major> c{sz};
	
	if ((vert_expand && horz_expand) || vert_expand) {
		for(std::size_t i = 0; i < a_m; ++i) {
			for(std::size_t j = 0; j < a_n; ++j) {
				(*at)(c, make_matrix_index(i, j)) = (*at)(v, j % v_sz);
			}
		}
	} else {
		//only horz_expand or no expansion
		for(std::size_t i = 0; i < a_n; ++i) {
			(*at)(c, make_matrix_index(0, i)) = (*at)(v, i % v_sz);
		}
	}
	
	return c;
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FN_EXPAND_IMPL_HBRS_MPL_HPP
