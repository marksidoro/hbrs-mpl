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

#ifndef HBRS_MPL_FN_MEAN_FWD_HBRS_MPL_HPP
#define HBRS_MPL_FN_MEAN_FWD_HBRS_MPL_HPP

#include <hbrs/mpl/core/preprocessor.hpp>

#include <hbrs/mpl/dt/smcs/fwd.hpp>
#include <hbrs/mpl/dt/rtsam/fwd.hpp>
#include <hbrs/mpl/dt/sm/fwd.hpp>
#include <hbrs/mpl/dt/ctsav/fwd.hpp>
#include <hbrs/mpl/dt/matrix_size/fwd.hpp>
#include <hbrs/mpl/dt/storage_order/fwd.hpp>

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
		smcs<
			sm<
				ctsav<Ring, SequenceSize>,
				matrix_size<hana::size_t<MatrixSizeM>, hana::size_t<MatrixSizeN>>,
				Order
			> const&
		> const& a) const;
};

struct mean_impl_smcs_rtsam {
	template <
		typename Ring,
		storage_order Order,
		typename std::enable_if_t<
			std::is_arithmetic<std::decay_t<Ring>>::value
		>* = nullptr
	>
	auto
	operator()(
		//TODO: extend to non-ref, non-const-ref, ...
		smcs<
			rtsam<Ring, Order> const&
		> const& a) const;
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_MEAN_IMPLS_HBRS_MPL boost::hana::make_tuple(                                                       \
		hbrs::mpl::detail::mean_impl_smcs_sm_ctsav_icsz{},                                                             \
		hbrs::mpl::detail::mean_impl_smcs_rtsam{}                                                                      \
	)

#endif // !HBRS_MPL_FN_MEAN_FWD_HBRS_MPL_HPP
