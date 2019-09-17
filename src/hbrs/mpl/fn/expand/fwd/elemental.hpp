/* Copyright (c) 2016-2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_FN_EXPAND_FWD_ELEMENTAL_HPP
#define HBRS_MPL_FN_EXPAND_FWD_ELEMENTAL_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
	#include <hbrs/mpl/dt/el_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/el_dist_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/el_vector/fwd.hpp>
	#include <hbrs/mpl/dt/el_dist_vector/fwd.hpp>
#endif

#include <hbrs/mpl/dt/smr/fwd.hpp>
#include <hbrs/mpl/dt/matrix_size/fwd.hpp>
#include <boost/hana/tuple.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
//TODO: Join with expand_impl_smr_ml_matrix!
struct expand_impl_smr_el_matrix {
	template<
		typename Matrix,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value
		>* = nullptr
	>
	auto
	operator()(
		smr<Matrix, El::Int> const& a,
		matrix_size<El::Int, El::Int> const& sz
	) const;
};

//TODO: Join with expand_impl_ml_row_vector!
struct expand_impl_el_row_vector {
	template<typename Ring>
	auto
	operator()(
		el_row_vector<Ring> const& v,
		matrix_size<El::Int, El::Int> const& sz
	) const;
};

struct expand_impl_el_column_vector {
	template<typename Ring>
	auto
	operator()(
		el_column_vector<Ring> const& v,
		matrix_size<El::Int, El::Int> const& sz
	) const;
};

struct expand_impl_el_dist_row_vector {
	template <typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
	constexpr auto
	operator()(
		el_dist_row_vector<Ring, Columnwise, Rowwise, Wrapping> const& v,
		matrix_size<El::Int, El::Int> const& sz
	) const;
};

struct expand_impl_el_dist_column_vector {
	template <typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
	constexpr auto
	operator()(
		el_dist_column_vector<Ring, Columnwise, Rowwise, Wrapping> const& v,
		matrix_size<El::Int, El::Int> const& sz
	) const;
};

#else
struct expand_impl_smr_el_matrix{};
struct expand_impl_el_row_vector{};
struct expand_impl_el_column_vector{};
struct expand_impl_el_dist_row_vector{};
struct expand_impl_el_dist_column_vector{};
#endif

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_EXPAND_IMPLS_ELEMENTAL boost::hana::make_tuple(                                                    \
		hbrs::mpl::detail::expand_impl_smr_el_matrix{},                                                                \
		hbrs::mpl::detail::expand_impl_el_row_vector{},                                                                \
		hbrs::mpl::detail::expand_impl_el_column_vector{},                                                             \
		hbrs::mpl::detail::expand_impl_el_dist_row_vector{},                                                           \
		hbrs::mpl::detail::expand_impl_el_dist_column_vector{}                                                         \
	)

#endif // !HBRS_MPL_FN_EXPAND_FWD_ELEMENTAL_HPP
