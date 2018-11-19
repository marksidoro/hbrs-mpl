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

#pragma once

#ifndef HBRS_MPL_FUSE_HBRS_MPL_FN_MEAN_HPP
#define HBRS_MPL_FUSE_HBRS_MPL_FN_MEAN_HPP

#include <hbrs/mpl/preprocessor/core.hpp>

#include <hbrs/mpl/fwd/dt/smcs.hpp>
#include <hbrs/mpl/fwd/dt/sm.hpp>
#include <hbrs/mpl/fwd/dt/ctsav.hpp>
#include <hbrs/mpl/fwd/dt/matrix_size.hpp>
#include <hbrs/mpl/fwd/dt/storage_order.hpp>

#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/dt/srv.hpp>
#include <hbrs/mpl/dt/exception.hpp>

#include <hbrs/mpl/fn/equal.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/at.hpp>

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

struct mean_impl_smcs_sm_ctsav_icsz {
	template <
		typename Ring, std::size_t SequenceSize,
		std::size_t MatrixSizeM, std::size_t MatrixSizeN,
		storage_order Order,
		typename std::enable_if_t<
			std::is_arithmetic<std::decay_t<Ring>>::value
		>* = nullptr
	>
	auto
	operator()(
		//TODO: extend to non-ref, non-const-ref, ...
		mpl::smcs<
			sm<
				ctsav<Ring, SequenceSize>,
				matrix_size<hana::size_t<MatrixSizeM>, hana::size_t<MatrixSizeN>>,
				Order
			> const&
		> const& a) const {
		typedef std::decay_t<Ring> _Ring_;
		
		using namespace hbrs::mpl;
		auto a_sz = (*size)(a.data());
		auto a_m = (*m)(a_sz);
		auto a_n = (*n)(a_sz);
		
		if ((*equal)(a_m, 0) || (*equal)(a_n, 0)) {
			BOOST_THROW_EXCEPTION(incompatible_matrix_exception{} << errinfo_matrix_size{a_sz});
		}
		
		//TODO: parallelize and optimize for column major vs row major storage order, possibly using BLAS/LAPACK functions
		
		// column mean
		srv<std::vector<_Ring_>> mean_{ std::vector<_Ring_>(a_n, 0) };
		
		if constexpr(Order == storage_order::row_major) {
			for(std::size_t i = 0; i < a_m; ++i) {
				for(std::size_t j = 0; j < a_n; ++j) {
					mean_.at(j) += (*at)(a.data(), make_matrix_index(i,j));
				}
			}
		} else {
			for(std::size_t j = 0; j < a_n; ++j) {
				for(std::size_t i = 0; i < a_m; ++i) {
					mean_.at(j) += (*at)(a.data(), make_matrix_index(i,j));
				}
			}
		}
		
		for(std::size_t j = 0; j < a_n; ++j) {
			mean_.at(j) /= a_m;
		}
		
		return mean_;
	}
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FUSE_HBRS_MPL_FN_MEAN_IMPLS boost::hana::make_tuple(                                                  \
		hbrs::mpl::detail::mean_impl_smcs_sm_ctsav_icsz{}                                                              \
	)

#endif // !HBRS_MPL_FUSE_HBRS_MPL_FN_MEAN_HPP
