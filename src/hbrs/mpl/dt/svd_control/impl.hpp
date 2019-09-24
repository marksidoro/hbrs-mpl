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

#ifndef HBRS_MPL_DT_SVD_CONTROL_IMPL_HPP
#define HBRS_MPL_DT_SVD_CONTROL_IMPL_HPP

#include "fwd.hpp"

#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/detail/is_braces_constructible.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN

template<typename DecomposeMode>
struct svd_control {
	template<
		typename DecomposeMode_ = DecomposeMode,
		typename std::enable_if_t<
			std::is_default_constructible<DecomposeMode_>::value
		>* = nullptr
	>
	constexpr
	svd_control() {}
	
	template<
		typename DecomposeMode_ = DecomposeMode,
		typename std::enable_if_t<
			detail::is_braces_constructible_v<DecomposeMode, DecomposeMode_> &&
			!std::is_base_of_v<svd_control, std::remove_reference_t<DecomposeMode_> >
		>* = nullptr
	>
	constexpr 
	svd_control(DecomposeMode_ && dm)
	: decompose_mode_{HBRS_MPL_FWD(dm)}
	{}
	
	constexpr 
	svd_control(svd_control const&) = default;
	constexpr 
	svd_control(svd_control &&) = default;
	
	constexpr svd_control&
	operator=(svd_control const&) = default;
	constexpr svd_control&
	operator=(svd_control &&) = default;
	
	constexpr decltype(auto)
	decompose_mode() & { return (decompose_mode_); };
	
	constexpr decltype(auto)
	decompose_mode() const& { return (decompose_mode_); };
	
	constexpr decltype(auto)
	decompose_mode() && { return HBRS_MPL_FWD(decompose_mode_); };
	
private:
	DecomposeMode decompose_mode_;
};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <typename DecomposeMode>
struct tag_of< hbrs::mpl::svd_control<DecomposeMode> > {
	using type = hbrs::mpl::svd_control_tag;
};

template <>
struct make_impl<hbrs::mpl::svd_control_tag> {
	template <typename DecomposeMode>
	static constexpr hbrs::mpl::svd_control<
		std::decay_t<DecomposeMode>
	>
	apply(DecomposeMode && dm) {
		return { HBRS_MPL_FWD(dm) };
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_SVD_CONTROL_IMPL_HPP
