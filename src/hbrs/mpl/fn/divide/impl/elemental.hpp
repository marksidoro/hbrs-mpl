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

#ifndef HBRS_MPL_FN_DIVIDE_IMPL_ELEMENTAL_HPP
#define HBRS_MPL_FN_DIVIDE_IMPL_ELEMENTAL_HPP

#include <hbrs/mpl/config.hpp>
#include <El.hpp>
#include <hbrs/mpl/dt/el_vector/fwd.hpp>
#include <hbrs/mpl/dt/el_dist_vector/fwd.hpp>
#include <hbrs/mpl/dt/el_dist_matrix.hpp> // TODO: Remove once dist_column_vector is correctly implemented
#include <boost/hana/tuple.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace mpl = hbrs::mpl;
namespace detail {

struct divide_impl_vector_scalar {
	template <
		typename Vector,
		typename Scalar,
		typename std::enable_if_t< 
			(
				std::is_same< hana::tag_of_t<Vector>, column_vector_tag >::value ||
				std::is_same< hana::tag_of_t<Vector>, row_vector_tag >::value
			) &&
			std::is_arithmetic<Scalar>::value
		>* = nullptr
	>
	auto
	operator()(Vector v, Scalar const& b) const {
		//TODO: Move to member function? Or replace with transform call?
		El::Scale(Scalar(1)/b, v.data());
		return v;
	}
};

struct divide_impl_dist_vector_scalar {
	template <
		typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping,
		typename Scalar,
		typename std::enable_if_t<
			std::is_arithmetic_v<Scalar>
		>* = nullptr
	>
	auto
	operator()(
		dist_column_vector<Ring, Columnwise, Rowwise, Wrapping> && v,
		Scalar const& b
	) const {
		//TODO: Move to member function? Or replace with transform call?
		typedef std::decay_t<Ring> _Ring_;
		El::Scale(_Ring_(1)/b, v.data());
		return HBRS_MPL_FWD(v);
	}
};

struct divide_impl_matrix_scalar {
	template <
		typename Ring,
		typename std::enable_if_t< 
			!std::is_const< Ring >::value
		>* = nullptr
	>
	auto
	operator()(matrix<Ring> a, Ring const& b) const {
		El::Scale(Ring(1)/b, a.data());
		return a;
	}
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_DIVIDE_IMPLS_ELEMENTAL boost::hana::make_tuple(                                                        \
		elemental::detail::divide_impl_vector_scalar{},                                                                \
		elemental::detail::divide_impl_dist_vector_scalar{},                                                           \
		elemental::detail::divide_impl_matrix_scalar{}                                                                 \
	)

#endif // !HBRS_MPL_FN_DIVIDE_IMPL_ELEMENTAL_HPP
