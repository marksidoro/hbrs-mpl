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

#ifndef HBRS_MPL_FN_ABSOLUTE_FWD_ELEMENTAL_HPP
#define HBRS_MPL_FN_ABSOLUTE_FWD_ELEMENTAL_HPP

#include <hbrs/mpl/config.hpp>

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
	#include <hbrs/mpl/dt/el_matrix/fwd.hpp>
#endif

#include <boost/hana/tuple.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

#ifdef HBRS_MPL_ENABLE_ELEMENTAL

struct absolute_impl_el_matrix {
	template <
		typename Ring,
		typename std::enable_if_t<
			std::is_arithmetic< Ring >::value /* TODO: absolute() is not yet implemented for El::Complex<> */
		>* = nullptr
	>
	auto
	operator()(el_matrix<Ring> const& a) const;
};

#else
struct absolute_impl_el_matrix {};
#endif

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_ABSOLUTE_IMPLS_ELEMENTAL boost::hana::make_tuple(                                                  \
		hbrs::mpl::detail::absolute_impl_el_matrix{}                                                                   \
	)

#endif // !HBRS_MPL_FN_ABSOLUTE_FWD_ELEMENTAL_HPP
