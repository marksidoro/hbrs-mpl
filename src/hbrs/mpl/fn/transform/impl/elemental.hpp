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

#ifndef HBRS_MPL_FN_TRANSFORM_IMPL_ELEMENTAL_HPP
#define HBRS_MPL_FN_TRANSFORM_IMPL_ELEMENTAL_HPP

#include "../fwd/elemental.hpp"
#ifdef HBRS_MPL_ENABLE_ELEMENTAL

#include <hbrs/mpl/dt/el_matrix.hpp>
#include <hbrs/mpl/dt/el_dist_matrix.hpp>
#include <hbrs/mpl/dt/el_vector.hpp>
#include <hbrs/mpl/dt/el_dist_vector.hpp>
#include <hbrs/mpl/dt/smc.hpp>
#include <hbrs/mpl/dt/smcs.hpp>
#include <hbrs/mpl/dt/smr.hpp>
#include <hbrs/mpl/dt/smrs.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <functional>
#include <vector>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

template<typename Ring, typename F>
void
transform_el(El::Matrix<Ring> const& from, El::Matrix<Ring> & to, F && f) {
	BOOST_ASSERT(from.Height() == to.Height());
	BOOST_ASSERT(from.Width() == to.Width());
		
	auto const from_ldim = from.LDim();
	auto const to_ldim = to.LDim();
	for(El::Int j=0; j < from.Width(); ++j) {
		for(El::Int i=0; i < from.Height(); ++i) {
			decltype(auto) from_ref = from.LockedBuffer()[i+j*from_ldim];
			decltype(auto) to_ref = to.Buffer()[i+j*to_ldim];
			to_ref = evaluate(f(from_ref));
		}
	}
}

template <
	typename Vector,
	typename F,
	typename std::enable_if_t<
		std::is_same< hana::tag_of_t<Vector>, el_column_vector_tag >::value ||
		std::is_same< hana::tag_of_t<Vector>, el_row_vector_tag >::value
		//TODO: Add invokable check for F?
	>*
>
auto
transform_impl_el_vector::operator()(Vector v, F && f) const {
	BOOST_ASSERT(!v.data().Locked());
	transform_el(v.data(), v.data(), HBRS_MPL_FWD(f));
	return v;
}

template <
	typename Vector,
	typename F,
	typename std::enable_if_t<
		std::is_same_v< hana::tag_of_t<Vector>, el_dist_column_vector_tag > ||
		std::is_same_v< hana::tag_of_t<Vector>, el_dist_row_vector_tag >
		//TODO: Add invokable check for F?
	>*
>
auto
transform_impl_el_dist_vector::operator()(Vector v, F && f) const {
	BOOST_ASSERT(!v.data().Locked());
	transform_el(v.data().Matrix(), v.data().Matrix(), HBRS_MPL_FWD(f));
	return v;
}

template <
	typename Ring,
	typename F,
	typename std::enable_if_t<
		!std::is_const_v<Ring>
		//TODO: Add invokable check for F?
	>*
>
auto
transform_impl_el_matrix::operator()(el_matrix<Ring> & a, F && f) const {
	transform_el(a.data(), a.data(), HBRS_MPL_FWD(f));
	return a;
}

template <
	typename Ring,
	typename F
	//TODO: Add invokable check for F?
>
auto
transform_impl_el_matrix::operator()(el_matrix<Ring> const& a, F && f) const {
	typedef std::decay_t<Ring> _Ring_;
	el_matrix<_Ring_> b{a.m(), a.n()};
	transform_el(a.data(), b.data(), HBRS_MPL_FWD(f));
	return b;
}

template <
	typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping,
	typename F,
	typename std::enable_if_t<
		!std::is_const_v<Ring>
		//TODO: Add invokable check for F?
	>*
>
auto
transform_impl_el_dist_matrix::operator()(el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping> & a, F && f) const {
	transform_el(a.data().Matrix(), a.data().Matrix(), HBRS_MPL_FWD(f));
	return a;
}

template <
	typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping,
	typename F
	//TODO: Add invokable check for F?
>
auto
transform_impl_el_dist_matrix::operator()(el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping> const& a, F && f) const {
	typedef std::decay_t<Ring> _Ring_;
	el_dist_matrix<_Ring_, Columnwise, Rowwise, Wrapping> b{{a.data().Grid(), a.m(), a.n()}};
	transform_el(a.data().LockedMatrix(), b.data().Matrix(), HBRS_MPL_FWD(f));
	return b;
}

template <
	typename Matrix,
	typename F,
	typename std::enable_if_t< 
		std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value
		//TODO: Add invokable check for F?
	>*
>
auto
transform_impl_smr_el_matrix::operator()(smr<Matrix, El::Int> const& a, F && f) const {
	typedef decltype(evaluate( f(a.at(0)) )) Ring;
	typedef std::decay_t<Ring> _Ring_;
	std::vector<_Ring_> b;
	b.reserve(a.length());
	
	for(El::Int i=0; i < a.length(); ++i) {
		b.push_back( evaluate(f(a.at(i))) );
	}
	
	return b;
}

template <
	typename Matrix,
	typename F,
	typename std::enable_if_t< 
		std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value
		//TODO: Add invokable check for F?
	>*
>
auto
transform_impl_smc_el_matrix::operator()(smc<Matrix, El::Int> const& a, F && f) const {
	typedef decltype(evaluate( f(a.at(0)) )) Ring;
	typedef std::decay_t<Ring> _Ring_;
	std::vector<_Ring_> b;
	b.reserve(a.length());
	
	for(El::Int j=0; j < a.length(); ++j) {
		b.push_back( evaluate(f(a.at(j))) );
	}
	
	return b;
}

template <typename Sequence, typename F>
static auto
transform_impl_smcs_smrs_el_matrix_impl(Sequence const& s, F && f) {
	typedef std::decay_t<decltype( evaluate( f(s.at(0)) ) )>  Result;
	auto sz = (*size)(s);
	std::vector<Result> b;
	b.reserve(sz);
	
	for(El::Int i=0; i < sz; ++i) {
		b.push_back( evaluate(f(s.at(i))) );
	}
	
	return b;
}

template <
	typename Matrix,
	typename F,
	typename std::enable_if_t< 
		std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value
		//TODO: Add invokable check for F?
	>*
>
decltype(auto)
transform_impl_smcs_smrs_el_matrix::operator()(smcs<Matrix> const& a, F && f) const {
	return transform_impl_smcs_smrs_el_matrix_impl(a, HBRS_MPL_FWD(f));
}

template <
	typename Matrix,
	typename F,
	typename std::enable_if_t< 
		std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value
		//TODO: Add invokable check for F?
	>*
>
decltype(auto)
transform_impl_smcs_smrs_el_matrix::operator()(smrs<Matrix> const& a, F && f) const {
	return transform_impl_smcs_smrs_el_matrix_impl(a, HBRS_MPL_FWD(f));
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
#endif // !HBRS_MPL_FN_TRANSFORM_IMPL_ELEMENTAL_HPP
