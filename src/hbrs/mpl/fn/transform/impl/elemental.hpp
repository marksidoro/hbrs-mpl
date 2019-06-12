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

#ifndef ELEMENTAL_FUSE_FN_TRANSFORM_HPP
#define ELEMENTAL_FUSE_FN_TRANSFORM_HPP

#include <elemental/config.hpp>
#include <elemental/dt/matrix.hpp>
#include <elemental/dt/dist_matrix.hpp>
#include <elemental/dt/vector.hpp>
#include <elemental/dt/dist_vector.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/dt/smc.hpp>
#include <hbrs/mpl/dt/smcs.hpp>
#include <hbrs/mpl/dt/smr.hpp>
#include <hbrs/mpl/dt/smrs.hpp>
#include <El.hpp>
#include <boost/hana/tuple.hpp>
#include <type_traits>
#include <functional>
#include <vector>

ELEMENTAL_NAMESPACE_BEGIN
namespace mpl = hbrs::mpl;
namespace detail {

template<typename Ring, typename F>
void
transform_Matrix(El::Matrix<Ring> const& from, El::Matrix<Ring> & to, F && f) {
	BOOST_ASSERT(from.Height() == to.Height());
	BOOST_ASSERT(from.Width() == to.Width());
		
	auto const from_ldim = from.LDim();
	auto const to_ldim = to.LDim();
	for(El::Int j=0; j < from.Width(); ++j) {
		for(El::Int i=0; i < from.Height(); ++i) {
			decltype(auto) from_ref = from.LockedBuffer()[i+j*from_ldim];
			decltype(auto) to_ref = to.Buffer()[i+j*to_ldim];
			to_ref = mpl::evaluate(f(from_ref));
		}
	}
}

struct transform_impl_vector {
	template <
		typename Vector,
		typename F,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Vector>, column_vector_tag >::value ||
			std::is_same< hana::tag_of_t<Vector>, row_vector_tag >::value
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	auto
	operator()(Vector v, F && f) const {
		using namespace hbrs::mpl;
		BOOST_ASSERT(!v.data().Locked());
		transform_Matrix(v.data(), v.data(), HBRS_MPL_FWD(f));
		return v;
	}
};

struct transform_impl_dist_vector {
	template <
		typename Vector,
		typename F,
		typename std::enable_if_t<
			std::is_same_v< hana::tag_of_t<Vector>, dist_column_vector_tag > ||
			std::is_same_v< hana::tag_of_t<Vector>, dist_row_vector_tag >
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	auto
	operator()(Vector v, F && f) const {
		using namespace hbrs::mpl;
		BOOST_ASSERT(!v.data().Locked());
		transform_Matrix(v.data().Matrix(), v.data().Matrix(), HBRS_MPL_FWD(f));
		return v;
	}
};

struct transform_impl_matrix {
	template <
		typename Ring,
		typename F,
		typename std::enable_if_t<
			!std::is_const_v<Ring>
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	auto
	operator()(matrix<Ring> & a, F && f) const {
		transform_Matrix(a.data(), a.data(), HBRS_MPL_FWD(f));
		return a;
	}
	
	template <
		typename Ring,
		typename F
		//TODO: Add invokable check for F?
	>
	auto
	operator()(matrix<Ring> const& a, F && f) const {
		typedef std::decay_t<Ring> _Ring_;
		matrix<_Ring_> b{a.m(), a.n()};
		transform_Matrix(a.data(), b.data(), HBRS_MPL_FWD(f));
		return b;
	}
};

struct transform_impl_dist_matrix {
	template <
		typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping,
		typename F,
		typename std::enable_if_t<
			!std::is_const_v<Ring>
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	auto
	operator()(dist_matrix<Ring, Columnwise, Rowwise, Wrapping> & a, F && f) const {
		transform_Matrix(a.data().Matrix(), a.data().Matrix(), HBRS_MPL_FWD(f));
		return a;
	}
	
	template <
		typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping,
		typename F
		//TODO: Add invokable check for F?
	>
	auto
	operator()(dist_matrix<Ring, Columnwise, Rowwise, Wrapping> const& a, F && f) const {
		typedef std::decay_t<Ring> _Ring_;
		dist_matrix<_Ring_, Columnwise, Rowwise, Wrapping> b{{a.data().Grid(), a.m(), a.n()}};
		transform_Matrix(a.data().LockedMatrix(), b.data().Matrix(), HBRS_MPL_FWD(f));
		return b;
	}
};

struct transform_impl_smr_matrix {
	template <
		typename Matrix,
		typename F,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, matrix_tag >::value
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	auto
	operator()(mpl::smr<Matrix, El::Int> const& a, F && f) const {
		typedef decltype(mpl::evaluate( f(a.at(0)) )) Ring;
		typedef std::decay_t<Ring> _Ring_;
		std::vector<_Ring_> b;
		b.reserve(a.length());
		
		for(El::Int i=0; i < a.length(); ++i) {
			b.push_back( mpl::evaluate(f(a.at(i))) );
		}
		
		return b;
	}
};

struct transform_impl_smc_matrix {
	template <
		typename Matrix,
		typename F,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, matrix_tag >::value
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	auto
	operator()(mpl::smc<Matrix, El::Int> const& a, F && f) const {
		typedef decltype(mpl::evaluate( f(a.at(0)) )) Ring;
		typedef std::decay_t<Ring> _Ring_;
		std::vector<_Ring_> b;
		b.reserve(a.length());
		
		for(El::Int j=0; j < a.length(); ++j) {
			b.push_back( mpl::evaluate(f(a.at(j))) );
		}
		
		return b;
	}
};

struct transform_impl_smcs_smrs_matrix {
	template <typename Sequence, typename F>
	static auto
	impl(Sequence const& s, F && f) {
		typedef std::decay_t<decltype( mpl::evaluate( f(s.at(0)) ) )>  Result;
		auto sz = (*mpl::size)(s);
		std::vector<Result> b;
		b.reserve(sz);
		
		for(El::Int i=0; i < sz; ++i) {
			b.push_back( mpl::evaluate(f(s.at(i))) );
		}
		
		return b;
	}
	
	template <
		typename Matrix,
		typename F,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, matrix_tag >::value
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	decltype(auto)
	operator()(mpl::smcs<Matrix> const& a, F && f) const {
		return impl(a, HBRS_MPL_FWD(f));
	}
	
	template <
		typename Matrix,
		typename F,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, matrix_tag >::value
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	decltype(auto)
	operator()(mpl::smrs<Matrix> const& a, F && f) const {
		return impl(a, HBRS_MPL_FWD(f));
	}
};

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_TRANSFORM_IMPLS boost::hana::make_tuple(                                                     \
		elemental::detail::transform_impl_vector{},                                                                    \
		elemental::detail::transform_impl_dist_vector{},                                                               \
		elemental::detail::transform_impl_matrix{},                                                                    \
		elemental::detail::transform_impl_dist_matrix{},                                                               \
		elemental::detail::transform_impl_smc_matrix{},                                                                \
		elemental::detail::transform_impl_smcs_smrs_matrix{},                                                          \
		elemental::detail::transform_impl_smr_matrix{}                                                                 \
	)

#endif // !ELEMENTAL_FUSE_FN_TRANSFORM_HPP
