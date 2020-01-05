/* Copyright (c) 2020 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_DT_PCA_FILTER_CONTROL_IMPL_HPP
#define HBRS_MPL_DT_PCA_FILTER_CONTROL_IMPL_HPP

#include "fwd.hpp"

#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/detail/is_braces_constructible.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN

template<typename PCAControl, typename KeepCentered>
struct pca_filter_control {
	template<
		typename PCAControl_ = PCAControl,
		typename KeepCentered_ = KeepCentered,
		typename std::enable_if_t<
			std::is_default_constructible_v<PCAControl_> &&
			std::is_default_constructible_v<KeepCentered_>
		>* = nullptr
	>
	constexpr
	pca_filter_control() {}
	
	template<
		typename PCAControl_ = PCAControl,
		typename KeepCentered_ = KeepCentered,
		typename std::enable_if_t<
			detail::is_braces_constructible_v<PCAControl, PCAControl_> &&
			detail::is_braces_constructible_v<KeepCentered, KeepCentered_>
		>* = nullptr
	>
	constexpr 
	pca_filter_control(PCAControl_ && pca_control, KeepCentered_ && keep_centered)
	: pca_control_{HBRS_MPL_FWD(pca_control)},
	  keep_centered_{HBRS_MPL_FWD(keep_centered)}
	{}
	
	constexpr 
	pca_filter_control(pca_filter_control const&) = default;
	constexpr 
	pca_filter_control(pca_filter_control &&) = default;
	
	constexpr pca_filter_control&
	operator=(pca_filter_control const&) = default;
	constexpr pca_filter_control&
	operator=(pca_filter_control &&) = default;
	
	constexpr decltype(auto)
	pca_control() & { return (pca_control_); };
	
	constexpr decltype(auto)
	pca_control() const& { return (pca_control_); };
	
	constexpr decltype(auto)
	pca_control() && { return HBRS_MPL_FWD(pca_control_); };
	
	constexpr decltype(auto)
	keep_centered() & { return (keep_centered_); };
	
	constexpr decltype(auto)
	keep_centered() const& { return (keep_centered_); };
	
	constexpr decltype(auto)
	keep_centered() && { return HBRS_MPL_FWD(keep_centered_); };
	
private:
	PCAControl pca_control_;
	KeepCentered keep_centered_;
};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <typename PCAControl, typename KeepCentered>
struct tag_of< hbrs::mpl::pca_filter_control<PCAControl, KeepCentered> > {
	using type = hbrs::mpl::pca_filter_control_tag;
};

template <>
struct make_impl<hbrs::mpl::pca_filter_control_tag> {
	template <typename PCAControl, typename KeepCentered>
	static constexpr hbrs::mpl::pca_filter_control<
		std::decay_t<PCAControl>,
		std::decay_t<KeepCentered>
	>
	apply(PCAControl && pca_control, KeepCentered && keep_centered) {
		return { HBRS_MPL_FWD(pca_control), HBRS_MPL_FWD(keep_centered) };
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_PCA_FILTER_CONTROL_IMPL_HPP
