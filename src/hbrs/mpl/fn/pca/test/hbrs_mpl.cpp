/* Copyright (c) 2018 Jakob Meng, <jakobmeng@web.de>
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

#define BOOST_TEST_MODULE pca_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/fn/pca.hpp>
#include <hbrs/mpl/detail/test.hpp>
#include <hbrs/mpl/detail/gather.hpp>
#include <hbrs/mpl/dt/ctsav.hpp>
#include <hbrs/mpl/dt/sm.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/plus.hpp>
#include <hbrs/mpl/fn/multiply.hpp>
#include <hbrs/mpl/fn/transpose.hpp>
#include <hbrs/mpl/fn/expand.hpp>
#include <hbrs/mpl/fn/select.hpp>
#ifdef HBRS_MPL_ENABLE_ELEMENTAL
	#include <hbrs/mpl/dt/el_matrix.hpp>
	#include <hbrs/mpl/dt/el_dist_matrix.hpp>
#endif
#ifdef HBRS_MPL_ENABLE_MATLAB
	#include <hbrs/mpl/dt/ml_matrix.hpp>
#endif
#include <boost/hana/tuple.hpp>
#include <boost/hana/transform.hpp>
#include <boost/hana/for_each.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/plus.hpp>
#include <boost/hana/minus.hpp>
#include <boost/hana/at.hpp>
#include <boost/hana/length.hpp>
#include <boost/hana/drop_back.hpp>
#include <boost/hana/greater_equal.hpp>
#include <boost/hana/cartesian_product.hpp>

#include <hbrs/mpl/detail/test.hpp>
#include <hbrs/mpl/detail/not_supported.hpp>

namespace utf = boost::unit_test;
namespace tt = boost::test_tools;

BOOST_AUTO_TEST_SUITE(pca_test)

using hbrs::mpl::detail::environment_fixture;
BOOST_TEST_GLOBAL_FIXTURE(environment_fixture);

BOOST_AUTO_TEST_CASE(pca_comparison,  * utf::tolerance(0.000000001)) {
	using namespace hbrs::mpl;
	
	static constexpr auto datasets = hana::make_tuple(
		make_sm(
			make_ctsav(detail::mat_a), make_matrix_size(hana::size_c<detail::mat_a_m>, hana::size_c<detail::mat_a_n>), row_major_c
		),
		make_sm(
			make_ctsav(detail::mat_g), make_matrix_size(hana::size_c<detail::mat_g_m>, hana::size_c<detail::mat_g_n>), row_major_c
		),
		make_sm(
			make_ctsav(detail::mat_j), make_matrix_size(hana::size_c<detail::mat_j_m>, hana::size_c<detail::mat_j_n>), row_major_c
		)
	);
	
	static constexpr auto dimensions = hana::make_tuple(
		hana::to_tuple(hana::make_range(hana::size_c<0>, hana::length(datasets))),
		hana::make_tuple(hana::true_c, hana::false_c) /* economy */,
		hana::make_tuple(hana::true_c, hana::false_c) /* center */,
		hana::make_tuple(hana::true_c, hana::false_c) /* normalize */
	);
	
	hana::for_each(hana::cartesian_product(dimensions), [](auto const& cfg) {
		auto const& dataset_nr = hana::at_c<0>(cfg);
		auto const& economy = hana::at_c<1>(cfg);
		auto const& center = hana::at_c<2>(cfg);
		auto const& normalize = hana::at_c<3>(cfg);
		
		BOOST_TEST_MESSAGE("dataset_nr=" << dataset_nr);
		BOOST_TEST_MESSAGE("economy=" << (economy ? "true" : "false"));
		BOOST_TEST_MESSAGE("center=" << (center? "true" : "false"));
		BOOST_TEST_MESSAGE("normalize=" << (normalize? "true" : "false"));
		
		auto const& dataset = hana::at(datasets, dataset_nr);
		
		auto funs = hana::drop_back(hana::make_tuple(
			#ifdef HBRS_MPL_ENABLE_MATLAB
			[](auto && a, auto economy, auto center, auto normalize) {
				BOOST_TEST_PASSPOINT();
				return detail::pca_impl_level0_ml_matrix{}(make_ml_matrix(HBRS_MPL_FWD(a)), {economy, center, normalize});
			},
			[](auto && a, auto economy, auto center, auto normalize) {
				BOOST_TEST_PASSPOINT();
				return detail::pca_impl_level1_ml_matrix{}(make_ml_matrix(HBRS_MPL_FWD(a)), {economy, center, normalize});
			},
			[](auto && a, auto economy, auto center, auto normalize) {
				BOOST_TEST_PASSPOINT();
				return detail::pca_impl_level2_ml_matrix{}(make_ml_matrix(HBRS_MPL_FWD(a)), {economy, center, normalize});
			},
			#endif
			
			#ifdef HBRS_MPL_ENABLE_ELEMENTAL
			[](auto && a, auto economy, auto center, auto normalize) {
				BOOST_TEST_PASSPOINT();
				return detail::pca_impl_el_matrix{}(make_el_matrix(HBRS_MPL_FWD(a)), {economy, center, normalize}); 
			},
			[](auto && a, auto economy, auto center, auto normalize) {
				BOOST_TEST_PASSPOINT();
				auto sz_ = (*size)(a);
				auto m_ = (*m)(sz_);
				auto n_ = (*n)(sz_);
				
				if constexpr(
					!economy && hana::value(m_) <= hana::value(n_)
					/* because this will do a zero svd which equals complete which is not supported by elemental */
				) {
					return detail::not_supported{};
				} else {
					static El::Grid grid{El::mpi::COMM_WORLD}; // grid is static because reference to grid is required by El::DistMatrix<...>
					return detail::pca_impl_el_dist_matrix{}(
						make_el_dist_matrix(
							grid,
							make_el_matrix(HBRS_MPL_FWD(a))
						),
						{economy, center, normalize}
					);
				}
			},
			#endif
			"SEQUENCE_TERMINATOR___REMOVED_BY_DROP_BACK"
		));
		
		int fun_nr = 0;
		auto results = hana::transform(
			funs,
			[&dataset, &economy, &center, &normalize, &fun_nr](auto f) {
				BOOST_TEST_MESSAGE("calling impl nr " << fun_nr);
				++fun_nr;
				return f(dataset, economy, center, normalize);
			}
		);
		
		if constexpr(hana::length(results) >= 2u) {
			auto compare = [&dataset, &economy, &center, &normalize, &results](auto i) {
				using hbrs::mpl::select;
				
				auto j = i+hana::ushort_c<1>;
				auto const& pca_i = hana::at(results, i);
				auto const& pca_j = hana::at(results, j);
				
				//TODO: Take ++j if j not supported!
				if constexpr(is_supported(pca_i) && is_supported(pca_j)) {
					auto sz_ = (*size)(dataset);
					auto m_ = (*m)(sz_);
					auto n_ = (*n)(sz_);
					
					BOOST_TEST_PASSPOINT();
					
					BOOST_TEST_MESSAGE("comparing pca_coeff of impl nr " << i << " and " << j);
					auto const& pca_coeff_i = (*at)(pca_i, pca_coeff{});
					auto const& pca_coeff_j = (*at)(pca_j, pca_coeff{});
					if ((m_ < n_) && !economy) {
						auto rng = std::make_pair(make_matrix_index(0,0), make_matrix_size((int)n_, (int)m_-1 /* TODO: Why n*DOF instead of n*m? */));
						auto pca_coeff_i_mxn = (*select)(pca_coeff_i, rng);
						auto pca_coeff_j_mxn = (*select)(pca_coeff_j, rng);
						HBRS_MPL_TEST_MMEQ(pca_coeff_i_mxn, pca_coeff_j_mxn, true);
					} else {
						HBRS_MPL_TEST_MMEQ(pca_coeff_i, pca_coeff_j, true);
					}
					
					BOOST_TEST_MESSAGE("comparing pca_score of impl nr " << i << " and " << j);
					HBRS_MPL_TEST_MMEQ((*at)(pca_i, pca_score{}),  (*at)(pca_j, pca_score{}), true);
					BOOST_TEST_MESSAGE("comparing pca_latent of impl nr " << i << " and " << j);
					HBRS_MPL_TEST_VVEQ((*at)(pca_i, pca_latent{}), (*at)(pca_j, pca_latent{}), false);
					BOOST_TEST_MESSAGE("comparing pca_mean of impl nr " << i << " and " << j);
					HBRS_MPL_TEST_VVEQ((*at)(pca_i, pca_mean{}),   (*at)(pca_j, pca_mean{}), false);
					BOOST_TEST_MESSAGE("comparing impl nr " << i << " and " << j << " done.");
					
					BOOST_TEST_PASSPOINT();
				} else {
					BOOST_TEST_MESSAGE("skip comparison of impl nr " << i << " and " << j);
				}
			};
			
			hana::for_each(
				hana::make_range(
					hana::size_c<0>, 
					hana::length(results)-hana::size_c<1>
				),
				compare
			);
		}
		
		{
			auto rebuilds = hana::transform(results, [](auto && pca_result) {
				if constexpr(is_not_supported(pca_result)) {
					return detail::not_supported{};
				} else {
					auto && coeff_ =  (*at) (pca_result, pca_coeff{});
					auto && score_ =  (*at) (pca_result, pca_score{});
// 					auto && latent_ = (*at) (pca_result, pca_latent{});
					auto && mean_ =   (*at) (pca_result, pca_mean{});
					
					auto centered = (*multiply)(score_, transpose(coeff_));
					return (*plus)(centered, expand(mean_, size(centered)));
				}
			});
			
			hana::for_each(
				hana::make_range(hana::ushort_c<0>, hana::length(results)),
				[&dataset, &rebuilds](auto i) {
					
					auto const& rebuild = hana::at(rebuilds, i);
					
					if constexpr(is_supported(rebuild)) {
						BOOST_TEST_MESSAGE("comparing original and reconstructed dataset of impl nr " << i);
						HBRS_MPL_TEST_MMEQ(dataset, rebuild, false);
					} else {
						BOOST_TEST_MESSAGE("skip comparison of unsupported dataset for impl nr " << i);
					}
				}
			);
			BOOST_TEST_MESSAGE("comparing original and reconstructed datasets done.");
		}
	});
	
}

BOOST_AUTO_TEST_SUITE_END()
