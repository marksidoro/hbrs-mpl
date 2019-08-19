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

template<typename Economy, typename Center>
struct pca_control {
	template<
		typename Economy_ = Economy,
		typename Center_ = Center,
		typename std::enable_if_t<
			std::is_default_constructible<Economy_>::value &&
			std::is_default_constructible<Center_>::value
		>* = nullptr
	>
	constexpr
	pca_control() {}
	
	template<typename Economy_, typename Center_>
	constexpr 
	pca_control(Economy_ && e, Center_ && c)
	: economy_{HBRS_MPL_FWD(e)},
	  center_{HBRS_MPL_FWD(c)}
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
	
private:
	Economy economy_;
	Center center_;
};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <typename Economy, typename Center>
struct tag_of< hbrs::mpl::pca_control<Economy, Center> > {
	using type = hbrs::mpl::pca_control_tag;
};

template <>
struct make_impl<hbrs::mpl::pca_control_tag> {
	template <typename Economy, typename Center>
	static constexpr hbrs::mpl::pca_control<
		std::decay_t<Economy>,
		std::decay_t<Center>
	>
	apply(Economy && e, Center && c) {
		return { HBRS_MPL_FWD(e), HBRS_MPL_FWD(c) };
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_PCA_CONTROL_IMPL_HPP
