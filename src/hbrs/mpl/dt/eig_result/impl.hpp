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

#ifndef HBRS_MPL_DT_EIG_RESULT_IMPL_HPP
#define HBRS_MPL_DT_EIG_RESULT_IMPL_HPP

#include "fwd.hpp"
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/detail/is_braces_constructible.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN

template<typename Eigenvalues, typename Eigenvectors>
struct eig_result {
	template<
		typename Eigenvalues_ = Eigenvalues,
		typename Eigenvectors_ = Eigenvectors,
		typename std::enable_if_t<
			std::is_default_constructible<Eigenvalues_>::value &&
			std::is_default_constructible<Eigenvectors_>::value
		>* = nullptr
	>
	constexpr
	eig_result() {}
	
	template<
		typename Eigenvalues_ = Eigenvalues,
		typename Eigenvectors_ = Eigenvectors,
		typename std::enable_if_t<
			detail::is_braces_constructible_v<Eigenvalues, Eigenvalues_> &&
			detail::is_braces_constructible_v<Eigenvectors, Eigenvectors_>
		>* = nullptr
	>
	constexpr 
	eig_result(Eigenvalues_ && eigenvalues, Eigenvectors_ && eigenvectors)
	: eigenvalues_{HBRS_MPL_FWD(eigenvalues)}, eigenvectors_{HBRS_MPL_FWD(eigenvectors)}
	{}
	
	constexpr 
	eig_result(eig_result const&) = default;
	constexpr 
	eig_result(eig_result &&) = default;
	
	constexpr eig_result&
	operator=(eig_result const&) = default;
	constexpr eig_result&
	operator=(eig_result &&) = default;
	
	constexpr decltype(auto)
	eigenvalues() & { return (eigenvalues_); };
	
	constexpr decltype(auto)
	eigenvalues() const& { return (eigenvalues_); };
	
	constexpr decltype(auto)
	eigenvalues() && { return HBRS_MPL_FWD(eigenvalues_); };
	
	constexpr decltype(auto)
	eigenvectors() & { return (eigenvectors_); };
	
	constexpr decltype(auto)
	eigenvectors() const& { return (eigenvectors_); };
	
	constexpr decltype(auto)
	eigenvectors() && { return HBRS_MPL_FWD(eigenvectors_); };

private:
	Eigenvalues eigenvalues_;
	Eigenvectors eigenvectors_;
};

struct eig_eigenvalues{};
struct eig_eigenvectors{};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <typename Eigenvalues, typename Eigenvectors>
struct tag_of< hbrs::mpl::eig_result<Eigenvalues, Eigenvectors> > {
	using type = hbrs::mpl::eig_result_tag;
};

template <>
struct make_impl<hbrs::mpl::eig_result_tag> {
	template <typename Eigenvalues, typename Eigenvectors>
	static constexpr hbrs::mpl::eig_result<
		std::decay_t<Eigenvalues>,
		std::decay_t<Eigenvectors>
	>
	apply(Eigenvalues && eigenvalues, Eigenvectors && eigenvectors) {
		return {
			HBRS_MPL_FWD(eigenvalues),
			HBRS_MPL_FWD(eigenvectors)
		};
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_EIG_RESULT_IMPL_HPP
