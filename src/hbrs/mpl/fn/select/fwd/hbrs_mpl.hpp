/* Copyright (c) 2019 Abdullah Güntepe, <abdullah@guentepe.com>
 * Copyright (c) 2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_FN_SELECT_FWD_HBRS_MPL_HPP
#define HBRS_MPL_FN_SELECT_FWD_HBRS_MPL_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/dt/rtsam/fwd.hpp>
#include <hbrs/mpl/dt/rtsacv/fwd.hpp>
#include <hbrs/mpl/dt/rtsarv/fwd.hpp>
#include <hbrs/mpl/dt/submatrix/fwd.hpp>
#include <hbrs/mpl/dt/range/fwd.hpp>
#include <hbrs/mpl/dt/matrix_index/fwd.hpp>
#include <hbrs/mpl/dt/matrix_size/fwd.hpp>
#include <boost/hana/ext/std/pair.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

struct select_impl_rtsam_range_index {
	template<
		typename Ring,
		storage_order Order
	>
	decltype(auto)
	operator()(
		rtsam<Ring,Order> const& a,
		std::pair<
			range<std::size_t,std::size_t>,
			std::size_t
		> const& rng
	) const;
	
	template<
		typename Ring,
		storage_order Order,
		typename Offset,
		typename Size
	>
	decltype(auto)
	operator()(
		submatrix<rtsam<Ring,Order>&,Offset,Size> const& a,
		std::pair<
			range<std::size_t,std::size_t>,
			std::size_t
		> const& rng
	) const;
};

struct select_impl_rtsam_index_range {
	template<
		typename Ring,
		storage_order Order
	>
	decltype(auto)
	operator()(
		rtsam<Ring,Order> const& a,
		std::pair<
			std::size_t,
			range<std::size_t,std::size_t>
		> const& rng
	) const;

	template<
		typename Ring,
		storage_order Order,
		typename Offset,
		typename Size
	>
	decltype(auto)
	operator()(
		submatrix<rtsam<Ring,Order>&, Offset,Size> const& a,
		std::pair<
			std::size_t,
			range<std::size_t,std::size_t>
		> const& rng
	) const;
};

struct select_impl_rtsam_range_range {
	template<typename Ring, storage_order Order>
	auto
	operator()(
		rtsam<Ring,Order>& a,
		std::pair<
			range<std::size_t, std::size_t>,
			range<std::size_t, std::size_t>
		> const& rngs
	) const;
};

struct select_impl_rtsam_index_offset {
	template<typename Ring, storage_order Order>
	decltype(auto)
	operator()(
		rtsam<Ring,Order> const& a,
		std::pair<
			matrix_index<std::size_t, std::size_t>,
			matrix_size<std::size_t, std::size_t>
		>  const& rng
	) const;
};

struct select_impl_rtsacv_range {
	template<typename Ring>
	auto
	operator() (
		rtsacv<Ring> const& v,
		range<std::size_t,std::size_t> const& rng
	) const;
};

struct select_impl_rtsarv_range {
	template<typename Ring>
	auto
	operator() (
		rtsarv<Ring> const& v,
		range<std::size_t,std::size_t> const& rng
	) const;
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_SELECT_IMPLS_HBRS_MPL boost::hana::make_tuple(                                                     \
		hbrs::mpl::detail::select_impl_rtsam_range_index{},                                                            \
		hbrs::mpl::detail::select_impl_rtsam_index_range{},                                                            \
		hbrs::mpl::detail::select_impl_rtsam_range_range{},                                                            \
		hbrs::mpl::detail::select_impl_rtsam_index_offset{},                                                           \
		hbrs::mpl::detail::select_impl_rtsacv_range{},                                                                 \
		hbrs::mpl::detail::select_impl_rtsarv_range{}                                                                  \
	)

#endif // !HBRS_MPL_FN_SELECT_FWD_HBRS_MPL_HPP
