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

#ifndef HBRS_MPL_FN_VERTCAT_FWD_ELEMENTAL_HPP
#define HBRS_MPL_FN_VERTCAT_FWD_ELEMENTAL_HPP

#include <hbrs/mpl/config.hpp>

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
	#include <hbrs/mpl/dt/el_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/smr/fwd.hpp>
#endif

#include <boost/hana/tuple.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

#ifdef HBRS_MPL_ENABLE_ELEMENTAL

struct vertcat_impl_smr_el_matrix_smr_el_matrix {
	template<
		typename MatrixA,
		typename MatrixB,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<MatrixA>, el_matrix_tag >::value && 
			std::is_same< hana::tag_of_t<MatrixB>, el_matrix_tag >::value && 
			std::is_same< std::decay_t<MatrixA>, std::decay_t<MatrixB> >::value
		>* = nullptr
	>
	auto
	operator()(
		smr<MatrixA, El::Int> const& a, 
		smr<MatrixB, El::Int> const& b
	) const;
};

struct vertcat_impl_el_matrix_smr_el_matrix {
	template<
		typename MatrixA,
		typename MatrixB,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<MatrixA>, el_matrix_tag >::value && 
			std::is_same< hana::tag_of_t<MatrixB>, el_matrix_tag >::value && 
			std::is_same< std::decay_t<MatrixA>, std::decay_t<MatrixB> >::value
		>* = nullptr
	>
	auto
	operator()(
		MatrixA const& a,
		smr<MatrixB, El::Int> const& b
	) const;
};

struct vertcat_impl_el_matrix_el_matrix {
	template<typename Ring>
	auto
	operator()(
		el_matrix<Ring> const& a,
		el_matrix<Ring> const& b
	) const;
};

#else
struct vertcat_impl_smr_el_matrix_smr_el_matrix {};
struct vertcat_impl_el_matrix_smr_el_matrix {};
struct vertcat_impl_el_matrix_el_matrix {};
#endif

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_VERTCAT_IMPLS_ELEMENTAL boost::hana::make_tuple(                                                   \
		hbrs::mpl::detail::vertcat_impl_smr_el_matrix_smr_el_matrix{},                                                 \
		hbrs::mpl::detail::vertcat_impl_el_matrix_smr_el_matrix{},                                                     \
		hbrs::mpl::detail::vertcat_impl_el_matrix_el_matrix{}                                                          \
	)

#endif // !HBRS_MPL_FN_VERTCAT_FWD_ELEMENTAL_HPP
