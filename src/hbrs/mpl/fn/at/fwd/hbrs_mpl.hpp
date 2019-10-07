/* Copyright (c) 2016-2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_FN_AT_FWD_HBRS_MPL_HPP
#define HBRS_MPL_FN_AT_FWD_HBRS_MPL_HPP

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/detail/function_object.hpp>

#include <hbrs/mpl/dt/ctsav/fwd.hpp>
#include <hbrs/mpl/dt/rtsav/fwd.hpp>
#include <hbrs/mpl/dt/submatrix/fwd.hpp>
#include <hbrs/mpl/dt/subsequence/fwd.hpp>
#include <hbrs/mpl/dt/sm/fwd.hpp>
#include <hbrs/mpl/dt/smc/fwd.hpp>
#include <hbrs/mpl/dt/smcs/fwd.hpp>
#include <hbrs/mpl/dt/smr/fwd.hpp>
#include <hbrs/mpl/dt/smrs/fwd.hpp>
#include <hbrs/mpl/dt/sms/fwd.hpp>
#include <hbrs/mpl/dt/srv/fwd.hpp>
#include <hbrs/mpl/dt/scv/fwd.hpp>
#include <hbrs/mpl/dt/ctsam/fwd.hpp>
#include <hbrs/mpl/dt/rtsam/fwd.hpp>
#include <hbrs/mpl/dt/rtsacv/fwd.hpp>
#include <hbrs/mpl/dt/rtsarv/fwd.hpp>
#include <hbrs/mpl/dt/zas/fwd.hpp>
#include <hbrs/mpl/dt/bidiag_result/fwd.hpp>
#include <hbrs/mpl/dt/svd_result/fwd.hpp>
#include <hbrs/mpl/dt/pca_result/fwd.hpp>
#include <hbrs/mpl/dt/pca_filter_result/fwd.hpp>
#include <hbrs/mpl/dt/dmd_result/fwd.hpp>
#include <hbrs/mpl/dt/eig_result/fwd.hpp>

#include <boost/hana/tuple.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

HBRS_MPL_DEC_FO_TRY_METHOD(at_impl_ctsav,    ctsav_tag,    operator[])
HBRS_MPL_DEC_FO_TRY_METHOD(at_impl_rtsav,    rtsav_tag,    operator[])
HBRS_MPL_DEC_FO_TRY_METHOD(at_impl_submatrix,   submatrix_tag,   at)
HBRS_MPL_DEC_FO_TRY_METHOD(at_impl_subsequence, subsequence_tag, at)
HBRS_MPL_DEC_FO_TRY_METHOD(at_impl_sm,       sm_tag,       at)
HBRS_MPL_DEC_FO_TRY_METHOD(at_impl_smr,      smr_tag,      at)
HBRS_MPL_DEC_FO_TRY_METHOD(at_impl_smrs,     smrs_tag,     at)
HBRS_MPL_DEC_FO_TRY_METHOD(at_impl_smc,      smc_tag,      at)
HBRS_MPL_DEC_FO_TRY_METHOD(at_impl_smcs,     smcs_tag,     at)
HBRS_MPL_DEC_FO_TRY_METHOD(at_impl_sms,      sms_tag,      at)
HBRS_MPL_DEC_FO_TRY_METHOD(at_impl_srv,      srv_tag,      at)
HBRS_MPL_DEC_FO_TRY_METHOD(at_impl_scv,      scv_tag,      at)
HBRS_MPL_DEC_FO_TRY_METHOD(at_impl_ctsam,    ctsam_tag,    at)
HBRS_MPL_DEC_FO_TRY_METHOD(at_impl_rtsam,    rtsam_tag,    at)
HBRS_MPL_DEC_FO_TRY_METHOD(at_impl_rtsacv,   rtsacv_tag,   at)
HBRS_MPL_DEC_FO_TRY_METHOD(at_impl_rtsarv,   rtsarv_tag,   at)
HBRS_MPL_DEC_FO_TRY_METHOD(at_impl_zas,      zas_tag,      at)

#define _HBRS_MPL_DEC_FO_NAMED_AT(impl_t, tag_t, named_t, method)                                                      \
	struct impl_t {                                                                                                    \
		template<                                                                                                      \
			typename T,                                                                                                \
			typename std::enable_if_t< std::is_same< hana::tag_of_t<T>, tag_t>::value >* = nullptr                     \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(T && t, named_t) const;                                                                             \
	};

_HBRS_MPL_DEC_FO_NAMED_AT(at_impl_bidiag_u, bidiag_result_tag, bidiag_u, u)
_HBRS_MPL_DEC_FO_NAMED_AT(at_impl_bidiag_b, bidiag_result_tag, bidiag_b, b)
_HBRS_MPL_DEC_FO_NAMED_AT(at_impl_bidiag_v, bidiag_result_tag, bidiag_v, v)
_HBRS_MPL_DEC_FO_NAMED_AT(at_impl_svd_u, svd_result_tag, svd_u, u)
_HBRS_MPL_DEC_FO_NAMED_AT(at_impl_svd_s, svd_result_tag, svd_s, s)
_HBRS_MPL_DEC_FO_NAMED_AT(at_impl_svd_v, svd_result_tag, svd_v, v)
_HBRS_MPL_DEC_FO_NAMED_AT(at_impl_pca_coeff,  pca_result_tag, pca_coeff,  coeff)
_HBRS_MPL_DEC_FO_NAMED_AT(at_impl_pca_score,  pca_result_tag, pca_score,  score)
_HBRS_MPL_DEC_FO_NAMED_AT(at_impl_pca_latent, pca_result_tag, pca_latent, latent)
_HBRS_MPL_DEC_FO_NAMED_AT(at_impl_pca_mean,   pca_result_tag, pca_mean,   mean)
_HBRS_MPL_DEC_FO_NAMED_AT(at_impl_pca_filter_data,   pca_filter_result_tag, pca_filter_data,   data)
_HBRS_MPL_DEC_FO_NAMED_AT(at_impl_pca_filter_latent, pca_filter_result_tag, pca_filter_latent, latent)
_HBRS_MPL_DEC_FO_NAMED_AT(at_impl_dmd_eigenvalues,  dmd_result_tag, dmd_eigenvalues,  eigenvalues)
_HBRS_MPL_DEC_FO_NAMED_AT(at_impl_dmd_modes,        dmd_result_tag, dmd_modes,        modes)
_HBRS_MPL_DEC_FO_NAMED_AT(at_impl_dmd_coefficients, dmd_result_tag, dmd_coefficients, coefficients)
_HBRS_MPL_DEC_FO_NAMED_AT(at_impl_eig_eigenvalues,  eig_result_tag, eig_eigenvalues,  eigenvalues)
_HBRS_MPL_DEC_FO_NAMED_AT(at_impl_eig_eigenvectors, eig_result_tag, eig_eigenvectors, eigenvectors)

#undef _HBRS_MPL_DEC_FO_NAMED_AT

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_AT_IMPLS_HBRS_MPL boost::hana::make_tuple(                                                         \
		hbrs::mpl::detail::at_impl_ctsav{},                                                                            \
		hbrs::mpl::detail::at_impl_rtsav{},                                                                            \
		hbrs::mpl::detail::at_impl_submatrix{},                                                                        \
		hbrs::mpl::detail::at_impl_subsequence{},                                                                      \
		hbrs::mpl::detail::at_impl_sm{},                                                                               \
		hbrs::mpl::detail::at_impl_smc{},                                                                              \
		hbrs::mpl::detail::at_impl_smcs{},                                                                             \
		hbrs::mpl::detail::at_impl_smr{},                                                                              \
		hbrs::mpl::detail::at_impl_smrs{},                                                                             \
		hbrs::mpl::detail::at_impl_sms{},                                                                              \
		hbrs::mpl::detail::at_impl_srv{},                                                                              \
		hbrs::mpl::detail::at_impl_scv{},                                                                              \
		hbrs::mpl::detail::at_impl_ctsam{},                                                                            \
		hbrs::mpl::detail::at_impl_rtsam{},                                                                            \
		hbrs::mpl::detail::at_impl_rtsacv{},                                                                           \
		hbrs::mpl::detail::at_impl_rtsarv{},                                                                           \
		hbrs::mpl::detail::at_impl_bidiag_u{},                                                                         \
		hbrs::mpl::detail::at_impl_bidiag_b{},                                                                         \
		hbrs::mpl::detail::at_impl_bidiag_v{},                                                                         \
		hbrs::mpl::detail::at_impl_svd_u{},                                                                            \
		hbrs::mpl::detail::at_impl_svd_s{},                                                                            \
		hbrs::mpl::detail::at_impl_svd_v{},                                                                            \
		hbrs::mpl::detail::at_impl_pca_coeff{},                                                                        \
		hbrs::mpl::detail::at_impl_pca_score{},                                                                        \
		hbrs::mpl::detail::at_impl_pca_latent{},                                                                       \
		hbrs::mpl::detail::at_impl_pca_mean{},                                                                         \
		hbrs::mpl::detail::at_impl_pca_filter_data{},                                                                  \
		hbrs::mpl::detail::at_impl_pca_filter_latent{},                                                                \
		hbrs::mpl::detail::at_impl_dmd_eigenvalues{},                                                                  \
		hbrs::mpl::detail::at_impl_dmd_modes{},                                                                        \
		hbrs::mpl::detail::at_impl_dmd_coefficients{},                                                                 \
		hbrs::mpl::detail::at_impl_eig_eigenvalues{},                                                                  \
		hbrs::mpl::detail::at_impl_eig_eigenvectors{},                                                                 \
		hbrs::mpl::detail::at_impl_zas{}                                                                               \
	)                                                                                                                  \

#endif // !HBRS_MPL_FN_AT_FWD_HBRS_MPL_HPP
