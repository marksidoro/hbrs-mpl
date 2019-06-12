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

#ifndef HBRS_MPL_DT_PCA_RESULT_HPP
#define HBRS_MPL_DT_PCA_RESULT_HPP

#include <hbrs/mpl/dt/pca_result/fwd.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN

template<typename Coeff, typename Score, typename Latent, typename Mean>
struct pca_result {
	
	template<
		typename Coeff_ = Coeff,
		typename Score_ = Score,
		typename Latent_ = Latent,
		typename Mean_ = Mean,
		typename std::enable_if_t<
			std::is_default_constructible<Coeff_>::value &&
			std::is_default_constructible<Score_>::value &&
			std::is_default_constructible<Latent_>::value &&
			std::is_default_constructible<Mean_>::value
		>* = nullptr
	>
	constexpr
	pca_result() {}
	
	template<typename Coeff_, typename Score_, typename Latent_, typename Mean_>
	constexpr 
	pca_result(Coeff_ && c, Score_ && s, Latent_ && l, Mean_ && m) 
	: coeff_{HBRS_MPL_FWD(c)}, 
		score_{HBRS_MPL_FWD(s)}, 
		latent_{HBRS_MPL_FWD(l)}, 
		mean_{HBRS_MPL_FWD(m)}
	{}
	
	constexpr 
	pca_result(pca_result const&) = default;
	constexpr 
	pca_result(pca_result &&) = default;
	
	constexpr pca_result&
	operator=(pca_result const&) = default;
	constexpr pca_result&
	operator=(pca_result &&) = default;
	
	constexpr decltype(auto)
	coeff() & { return (coeff_); };
	
	constexpr decltype(auto)
	coeff() const& { return (coeff_); };
	
	constexpr decltype(auto)
	coeff() && { return HBRS_MPL_FWD(coeff_); };
	
	constexpr decltype(auto)
	score() & { return (score_); };
	
	constexpr decltype(auto)
	score() const& { return (score_); };
	
	constexpr decltype(auto)
	score() && { return HBRS_MPL_FWD(score_); };
	
	constexpr decltype(auto)
	latent() & { return (latent_); };
	
	constexpr decltype(auto)
	latent() const& { return (latent_); };
	
	constexpr decltype(auto)
	latent() && { return HBRS_MPL_FWD(latent_); };
	
	constexpr decltype(auto)
	mean() & { return (mean_); };
	
	constexpr decltype(auto)
	mean() const& { return (mean_); };
	
	constexpr decltype(auto)
	mean() && { return HBRS_MPL_FWD(mean_); };
	
private:
	Coeff coeff_;
	Score score_;
	Latent latent_; 
	Mean mean_;
};

struct pca_coeff{};
struct pca_score{};
struct pca_latent{};
struct pca_mean{};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <typename Coeff, typename Score, typename Latent, typename Mean>
struct tag_of< hbrs::mpl::pca_result<Coeff, Score, Latent, Mean> > {
	using type = hbrs::mpl::pca_result_tag;
};

template <>
struct make_impl<hbrs::mpl::pca_result_tag> {
	template <typename Coeff, typename Score, typename Latent, typename Mean>
	static constexpr hbrs::mpl::pca_result<
		std::decay_t<Coeff>, 
		std::decay_t<Score>, 
		std::decay_t<Latent>,
		std::decay_t<Mean>
	>
	apply(Coeff && c, Score && s, Latent && l, Mean && m) {
		return {
			HBRS_MPL_FWD(c),
			HBRS_MPL_FWD(s),
			HBRS_MPL_FWD(l),
			HBRS_MPL_FWD(m)
		};
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_PCA_RESULT_HPP
