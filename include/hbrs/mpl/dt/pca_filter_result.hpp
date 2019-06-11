/* Copyright (c) 2016 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_DT_PCA_FILTER_RESULT_HPP
#define HBRS_MPL_DT_PCA_FILTER_RESULT_HPP

#include <hbrs/mpl/fwd/dt/pca_filter_result.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <hbrs/mpl/preprocessor/core.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN

template<typename Data, typename Latent>
struct pca_filter_result {
	template<
		typename Data_ = Data,
		typename Latent_ = Latent,
		typename std::enable_if_t<
			std::is_default_constructible<Data_>::value &&
			std::is_default_constructible<Latent_>::value
		>* = nullptr
	>
	constexpr
	pca_filter_result() {}
	
	template<typename Data_, typename Latent_>
	constexpr 
	pca_filter_result(Data_ && d, Latent_ && l)
	: data_{HBRS_MPL_FWD(d)}, latent_{HBRS_MPL_FWD(l)}
	{}
	
	constexpr 
	pca_filter_result(pca_filter_result const&) = default;
	constexpr 
	pca_filter_result(pca_filter_result &&) = default;
	
	constexpr pca_filter_result&
	operator=(pca_filter_result const&) = default;
	constexpr pca_filter_result&
	operator=(pca_filter_result &&) = default;
	
	constexpr decltype(auto)
	data() & { return (data_); };
	
	constexpr decltype(auto)
	data() const& { return (data_); };
	
	constexpr decltype(auto)
	data() && { return HBRS_MPL_FWD(data_); };
	
	constexpr decltype(auto)
	latent() & { return (latent_); };
	
	constexpr decltype(auto)
	latent() const& { return (latent_); };
	
	constexpr decltype(auto)
	latent() && { return HBRS_MPL_FWD(latent_); };
	
private:
	Data data_;
	Latent latent_;
};

struct pca_filter_data{};
struct pca_filter_latent{};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <typename Data, typename Latent>
struct tag_of< hbrs::mpl::pca_filter_result<Data, Latent> > {
	using type = hbrs::mpl::pca_filter_result_tag;
};

template <>
struct make_impl<hbrs::mpl::pca_filter_result_tag> {
	template <typename Data, typename Latent>
	static constexpr hbrs::mpl::pca_filter_result<
		std::decay_t<Data>,
		std::decay_t<Latent>
	>
	apply(Data && d, Latent && l) {
		return {
			HBRS_MPL_FWD(d),
			HBRS_MPL_FWD(l)
		};
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_PCA_FILTER_RESULT_HPP
