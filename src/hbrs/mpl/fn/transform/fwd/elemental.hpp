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

#ifndef HBRS_MPL_FN_TRANSFORM_FWD_ELEMENTAL_HPP
#define HBRS_MPL_FN_TRANSFORM_FWD_ELEMENTAL_HPP

#include <hbrs/mpl/config.hpp>

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
	#include <hbrs/mpl/dt/el_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/el_dist_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/el_vector/fwd.hpp>
	#include <hbrs/mpl/dt/el_dist_vector/fwd.hpp>
	#include <hbrs/mpl/dt/smc/fwd.hpp>
	#include <hbrs/mpl/dt/smcs/fwd.hpp>
	#include <hbrs/mpl/dt/smr/fwd.hpp>
	#include <hbrs/mpl/dt/smrs/fwd.hpp>
#endif

#include <boost/hana/tuple.hpp>
#include <type_traits>
#include <functional>
#include <vector>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

#ifdef HBRS_MPL_ENABLE_ELEMENTAL

struct transform_impl_el_vector {
	template <
		typename Vector,
		typename F,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Vector>, el_column_vector_tag >::value ||
			std::is_same< hana::tag_of_t<Vector>, el_row_vector_tag >::value
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	auto
	operator()(Vector v, F && f) const;
};

struct transform_impl_el_dist_vector {
	template <
		typename Vector,
		typename F,
		typename std::enable_if_t<
			std::is_same_v< hana::tag_of_t<Vector>, el_dist_column_vector_tag > ||
			std::is_same_v< hana::tag_of_t<Vector>, el_dist_row_vector_tag >
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	auto
	operator()(Vector v, F && f) const;
};

struct transform_impl_el_matrix {
	template <
		typename Ring,
		typename F,
		typename std::enable_if_t<
			!std::is_const_v<Ring>
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	auto
	operator()(el_matrix<Ring> & a, F && f) const;
	
	template <
		typename Ring,
		typename F
		//TODO: Add invokable check for F?
	>
	auto
	operator()(el_matrix<Ring> const& a, F && f) const;
};

struct transform_impl_el_dist_matrix {
	template <
		typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping,
		typename F,
		typename std::enable_if_t<
			!std::is_const_v<Ring>
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	auto
	operator()(el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping> & a, F && f) const;
	
	template <
		typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping,
		typename F
		//TODO: Add invokable check for F?
	>
	auto
	operator()(el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping> const& a, F && f) const;
};

struct transform_impl_smr_el_matrix {
	template <
		typename Matrix,
		typename F,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	auto
	operator()(smr<Matrix, El::Int> const& a, F && f) const;
};

struct transform_impl_smc_el_matrix {
	template <
		typename Matrix,
		typename F,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	auto
	operator()(smc<Matrix, El::Int> const& a, F && f) const;
};

struct transform_impl_smcs_smrs_el_matrix {
	template <
		typename Matrix,
		typename F,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	decltype(auto)
	operator()(smcs<Matrix> const& a, F && f) const;
	
	template <
		typename Matrix,
		typename F,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	decltype(auto)
	operator()(smrs<Matrix> const& a, F && f) const;
};

#else
struct transform_impl_el_vector {};
struct transform_impl_el_dist_vector {};
struct transform_impl_el_matrix {};
struct transform_impl_el_dist_matrix {};
struct transform_impl_smr_el_matrix {};
struct transform_impl_smc_el_matrix {};
struct transform_impl_smcs_smrs_el_matrix {};
#endif

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_TRANSFORM_IMPLS_ELEMENTAL boost::hana::make_tuple(                                                 \
		hbrs::mpl::detail::transform_impl_el_vector{},                                                                 \
		hbrs::mpl::detail::transform_impl_el_dist_vector{},                                                            \
		hbrs::mpl::detail::transform_impl_el_matrix{},                                                                 \
		hbrs::mpl::detail::transform_impl_el_dist_matrix{},                                                            \
		hbrs::mpl::detail::transform_impl_smc_el_matrix{},                                                             \
		hbrs::mpl::detail::transform_impl_smcs_smrs_el_matrix{},                                                       \
		hbrs::mpl::detail::transform_impl_smr_el_matrix{}                                                              \
	)

#endif // !HBRS_MPL_FN_TRANSFORM_FWD_ELEMENTAL_HPP
