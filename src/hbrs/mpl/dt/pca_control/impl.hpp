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

#ifndef HBRS_MPL_DT_PCA_CONTROL_IMPL_HPP
#define HBRS_MPL_DT_PCA_CONTROL_IMPL_HPP

#include "fwd.hpp"

#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN

template<typename Economy, typename Center, typename Normalize>
struct pca_control {
	template<
		typename Economy_ = Economy,
		typename Center_ = Center,
		typename Normalize_ = Normalize,
		typename std::enable_if_t<
			std::is_default_constructible_v<Economy_> &&
			std::is_default_constructible_v<Center_> &&
			std::is_default_constructible_v<Normalize_>
		>* = nullptr
	>
	constexpr
	pca_control() {}
	
	template<typename Economy_, typename Center_, typename Normalize_>
	constexpr 
	pca_control(Economy_ && e, Center_ && c, Normalize_ && n)
	: economy_{HBRS_MPL_FWD(e)},
	  center_{HBRS_MPL_FWD(c)},
	  normalize_{HBRS_MPL_FWD(n)}
	{}
	
	constexpr 
	pca_control(pca_control const&) = default;
	constexpr 
	pca_control(pca_control &&) = default;
	
	constexpr pca_control&
	operator=(pca_control const&) = default;
	constexpr pca_control&
	operator=(pca_control &&) = default;
	
	constexpr decltype(auto)
	economy() & { return (economy_); };
	
	constexpr decltype(auto)
	economy() const& { return (economy_); };
	
	constexpr decltype(auto)
	economy() && { return HBRS_MPL_FWD(economy_); };
	
	constexpr decltype(auto)
	center() & { return (center_); };
	
	constexpr decltype(auto)
	center() const& { return (center_); };
	
	constexpr decltype(auto)
	center() && { return HBRS_MPL_FWD(center_); };
	
	constexpr decltype(auto)
	normalize() & { return (normalize_); };
	
	constexpr decltype(auto)
	normalize() const& { return (normalize_); };
	
	constexpr decltype(auto)
	normalize() && { return HBRS_MPL_FWD(normalize_); };
	
private:
	Economy economy_;
	Center center_;
	Normalize normalize_;
};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <typename Economy, typename Center, typename Normalize>
struct tag_of< hbrs::mpl::pca_control<Economy, Center, Normalize> > {
	using type = hbrs::mpl::pca_control_tag;
};

template <>
struct make_impl<hbrs::mpl::pca_control_tag> {
	template <typename Economy, typename Center, typename Normalize>
	static constexpr hbrs::mpl::pca_control<
		std::decay_t<Economy>,
		std::decay_t<Center>,
		std::decay_t<Normalize>
	>
	apply(Economy && e, Center && c, Normalize && n) {
		return { HBRS_MPL_FWD(e), HBRS_MPL_FWD(c), HBRS_MPL_FWD(n) };
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_PCA_CONTROL_IMPL_HPP
