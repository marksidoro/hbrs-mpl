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

#ifndef HBRS_MPL_FN_MEAN_IMPL_HBRS_MPL_HPP
#define HBRS_MPL_FN_MEAN_IMPL_HBRS_MPL_HPP

#include "../fwd/hbrs_mpl.hpp"

#include <hbrs/mpl/core/preprocessor.hpp>

#include <hbrs/mpl/dt/smcs.hpp>
#include <hbrs/mpl/dt/rtsam.hpp>
#include <hbrs/mpl/dt/sm.hpp>
#include <hbrs/mpl/dt/ctsav.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>
#include <hbrs/mpl/dt/storage_order.hpp>

#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/dt/srv.hpp>
#include <hbrs/mpl/dt/exception.hpp>

#include <hbrs/mpl/fn/equal.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/at.hpp>

#include <boost/hana/integral_constant.hpp>


HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template<typename Columns, typename Ring, storage_order Order>
auto
mean_impl_smcs_matrix(Columns && cols, hana::basic_type<Ring>, storage_order_<Order>) {
	decltype(auto) a = HBRS_MPL_FWD(cols).data();
	auto a_sz = (*size)(a);
	auto a_m = (*m)(a_sz);
	auto a_n = (*n)(a_sz);
	
	if ((*equal)(a_m, 0) || (*equal)(a_n, 0)) {
		BOOST_THROW_EXCEPTION(incompatible_matrix_exception{} << errinfo_matrix_size{a_sz});
	}
	
	//TODO: parallelize and optimize for column major vs row major storage order, possibly using BLAS/LAPACK functions
	
	// column mean
	srv<std::vector<Ring>> mean_{ std::vector<Ring>(a_n, 0) };
	
	if constexpr(Order == storage_order::row_major) {
		for(std::size_t i = 0; i < a_m; ++i) {
			for(std::size_t j = 0; j < a_n; ++j) {
				mean_.at(j) += (*at)(HBRS_MPL_FWD(a), make_matrix_index(i,j));
			}
		}
	} else {
		for(std::size_t j = 0; j < a_n; ++j) {
			for(std::size_t i = 0; i < a_m; ++i) {
				mean_.at(j) += (*at)(HBRS_MPL_FWD(a), make_matrix_index(i,j));
			}
		}
	}
	
	for(std::size_t j = 0; j < a_n; ++j) {
		mean_.at(j) /= a_m;
	}
	
	return mean_;
}


template <
	typename Ring, std::size_t SequenceSize,
	std::size_t MatrixSizeM, std::size_t MatrixSizeN,
	storage_order Order,
	typename std::enable_if_t<
		std::is_arithmetic<std::decay_t<Ring>>::value
	>*
>
auto
mean_impl_smcs_sm_ctsav_icsz::operator()(
	//TODO: extend to non-ref, non-const-ref, ...
	smcs<
		sm<
			ctsav<Ring, SequenceSize>,
			matrix_size<hana::size_t<MatrixSizeM>, hana::size_t<MatrixSizeN>>,
			Order
		> const&
	> const& a) const {
	typedef std::decay_t<Ring> _Ring_;
	return mean_impl_smcs_matrix(a, hana::type_c<_Ring_>, storage_order_c<Order>);
}

template <
	typename Ring,
	storage_order Order,
	typename std::enable_if_t<
		std::is_arithmetic<std::decay_t<Ring>>::value
	>*
>
auto
mean_impl_smcs_rtsam::operator()(
	//TODO: extend to non-ref, non-const-ref, ...
	smcs<
		rtsam<Ring, Order> const&
	> const& a) const {
	typedef std::decay_t<Ring> _Ring_;
	return mean_impl_smcs_matrix(a, hana::type_c<_Ring_>, storage_order_c<Order>);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FN_MEAN_IMPL_HBRS_MPL_HPP
