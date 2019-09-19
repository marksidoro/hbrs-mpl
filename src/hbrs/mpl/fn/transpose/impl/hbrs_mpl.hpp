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

#ifndef HBRS_MPL_FN_TRANSPOSE_IMPL_HBRS_MPL_HPP
#define HBRS_MPL_FN_TRANSPOSE_IMPL_HBRS_MPL_HPP

#include "../fwd/hbrs_mpl.hpp"

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/dt/srv.hpp>
#include <hbrs/mpl/dt/scv.hpp>
#include <hbrs/mpl/dt/rtsam.hpp>
#include <hbrs/mpl/dt/submatrix.hpp>
#include <hbrs/mpl/dt/rtsacv.hpp>
#include <hbrs/mpl/dt/rtsarv.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template <
	typename Vector,
	typename std::enable_if_t<
		std::is_same< hana::tag_of_t<Vector>, srv_tag>::value
	>*
>
constexpr auto
transpose_impl_srv::operator()(Vector && v) const {
	return make_scv(HBRS_MPL_FWD(v));
}

template <
	typename Vector,
	typename std::enable_if_t<
		std::is_same< hana::tag_of_t<Vector>, scv_tag>::value
	>*
>
constexpr auto 
transpose_impl_scv::operator()(Vector && v) const {
	return make_srv(HBRS_MPL_FWD(v));
}

template<typename Matrix, typename Ring, storage_order Order>
static auto
transpose_rtsam_impl(Matrix const& m_, hana::basic_type<Ring>, storage_order_<Order>) {
	typedef std::decay_t<Ring> _Ring_;
	rtsam<
		_Ring_,
		Order == storage_order::row_major ? storage_order::column_major : storage_order::row_major
	> transposed { (*n)(size(m_)), (*m)(size(m_)) };
	
	for (std::size_t i = 0; i < (*m)(size(transposed)); ++i) {
		for (std::size_t j = 0; j < (*n)(size(transposed)); ++j) {
			transposed.at(make_matrix_index(i, j)) = (*at)(m_, make_matrix_index(j, i));
		}
	}
	return transposed;
}

template<
	typename Ring,
	storage_order Order
>
decltype(auto)
transpose_impl_rtsam::operator()(rtsam<Ring,Order> const& m_) const {
	return transpose_rtsam_impl(m_, hana::type_c<Ring>, storage_order_c<Order>);
}

template<
	typename Ring,
	storage_order Order,
	typename Offset,
	typename Size
>
decltype(auto)
transpose_impl_rtsam::operator()(submatrix<rtsam<Ring,Order>&, Offset,Size> const& m_) const {
	return transpose_rtsam_impl(m_, hana::type_c<Ring>, storage_order_c<Order>);
}

template<
	typename Ring,
	typename std::enable_if_t<
		!std::is_const_v< std::remove_reference_t<Ring> >
	>*
>
rtsarv<Ring>
transpose_impl_rtsacv::operator()(rtsacv<Ring> const& v) const {
	return {v.data()};
}

template<
	typename Ring,
	typename std::enable_if_t<
		!std::is_const_v< std::remove_reference_t<Ring> >
	>*
>
rtsacv<Ring>
transpose_impl_rtsarv::operator()(rtsarv<Ring> const& v) const {
	return {v.data()};
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FN_TRANSPOSE_IMPL_HBRS_MPL_HPP
