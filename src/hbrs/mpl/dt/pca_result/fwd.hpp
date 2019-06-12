
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

#ifndef HBRS_MPL_FWD_DT_PCA_RESULT_HPP
#define HBRS_MPL_FWD_DT_PCA_RESULT_HPP

#include <hbrs/mpl/config.hpp>
#include <boost/hana/fwd/core/make.hpp>
#include <boost/hana/fwd/core/to.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;

template<typename Coeff, typename Score, typename Latent, typename Mean>
struct pca_result;
struct pca_result_tag {};
constexpr auto make_pca_result = hana::make<pca_result_tag>;
constexpr auto to_pca_result = hana::to<pca_result_tag>;

struct pca_coeff;
struct pca_score;
struct pca_latent;
struct pca_mean;

HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FWD_DT_PCA_RESULT_HPP
