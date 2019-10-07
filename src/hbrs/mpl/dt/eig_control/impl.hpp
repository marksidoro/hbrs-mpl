/* Copyright (c) 2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_DT_EIG_CONTROL_IMPL_HPP
#define HBRS_MPL_DT_EIG_CONTROL_IMPL_HPP

#include "fwd.hpp"

#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/detail/is_braces_constructible.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN

template<typename>
struct eig_control {
	template<typename = void>
	constexpr
	eig_control() {}
	
	constexpr 
	eig_control(eig_control const&) = default;
	constexpr 
	eig_control(eig_control &&) = default;
	
	constexpr eig_control&
	operator=(eig_control const&) = default;
	constexpr eig_control&
	operator=(eig_control &&) = default;
};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <>
struct tag_of< hbrs::mpl::eig_control<> > {
	using type = hbrs::mpl::eig_control_tag;
};

template <>
struct make_impl<hbrs::mpl::eig_control_tag> {
	static constexpr hbrs::mpl::eig_control<>
	apply() {
		return {};
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_EIG_CONTROL_IMPL_HPP
