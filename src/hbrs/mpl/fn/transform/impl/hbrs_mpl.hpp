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

#ifndef HBRS_MPL_FN_TRANSFORM_IMPL_HBRS_MPL_HPP
#define HBRS_MPL_FN_TRANSFORM_IMPL_HBRS_MPL_HPP

#include "../fwd/hbrs_mpl.hpp"

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/detail/transform_sequence.hpp>
#include <hbrs/mpl/dt/zas.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template <
	typename S,
	typename F,
	typename std::enable_if_t<
		boost::mpl::if_c<
			std::is_same< hana::tag_of_t<S>, zas_tag >::value,
			boost::mpl::and_<
				test_args<gat_c<0>::apply<std::decay_t<S>>>,
				test_args<gat_c<1>::apply<std::decay_t<S>>>
			>,
			std::false_type
		>::type::value
		//TODO: Add invokable check for F?
	>*
>
constexpr decltype(auto)
transform_impl_zas_std_array_vector_irange::operator()(S && s, F && f) const {
	return transform_homogenous_sequence(HBRS_MPL_FWD(s), HBRS_MPL_FWD(f), (*size)(s));
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FN_TRANSFORM_IMPL_HBRS_MPL_HPP
