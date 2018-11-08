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

#include <hbrs/mpl/fn/pca.hpp>

#include <hbrs/mpl/dt/ctsav.hpp>
#include <hbrs/mpl/dt/sm.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/plus.hpp>
#include <hbrs/mpl/fn/multiply.hpp>
#include <hbrs/mpl/fn/transpose.hpp>
#include <hbrs/mpl/fn/expand.hpp>
#include <hbrs/mpl/fn/select.hpp>
#ifdef HBRS_MPL_ENABLE_ADDON_ELEMENTAL
	#include <elemental/dt/matrix.hpp>
#endif
#ifdef HBRS_MPL_ENABLE_ADDON_MATLAB
	#include <matlab/dt/matrix.hpp>
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

#include "../data.hpp"
#include "../detail.hpp"

#define BOOST_TEST_MODULE pca_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

namespace utf = boost::unit_test;
namespace tt = boost::test_tools;

BOOST_AUTO_TEST_SUITE(pca_test)

BOOST_AUTO_TEST_CASE(pca_comparison,  * utf::tolerance(0.000000001)) {
	using namespace hbrs::mpl;
	
	static constexpr auto datasets = hana::make_tuple(
		make_sm(
			make_ctsav(test::mat_a), make_matrix_size(hana::size_c<test::mat_a_m>, hana::size_c<test::mat_a_n>), row_major_c
		),
		make_sm(
			make_ctsav(test::mat_g), make_matrix_size(hana::size_c<test::mat_g_m>, hana::size_c<test::mat_g_n>), row_major_c
		),
		make_sm(
			make_ctsav(test::mat_j), make_matrix_size(hana::size_c<test::mat_j_m>, hana::size_c<test::mat_j_n>), row_major_c
		)
	);
	
	std::size_t dataset_nr = 0;
	hana::for_each(datasets, [&dataset_nr](auto const& dataset) {
		BOOST_TEST_MESSAGE("dataset_nr=" << dataset_nr);
		for(bool economy : { true, false }) {
			BOOST_TEST_MESSAGE("economy=" << (economy ? "true" : "false"));
			
			auto funs = hana::drop_back(hana::make_tuple(
				#ifdef HBRS_MPL_ENABLE_ADDON_MATLAB
				[](auto && a, auto economy) {
					return matlab::detail::pca_impl_level0{}(matlab::make_matrix(HBRS_MPL_FWD(a)), economy);
				},
// 				[](auto && a, auto economy) {
// 					return matlab::detail::pca_impl_level1{}(matlab::make_matrix(HBRS_MPL_FWD(a)), economy);
// 				},
				[](auto && a, auto economy) {
					return matlab::detail::pca_impl_level2{}(matlab::make_matrix(HBRS_MPL_FWD(a)), economy);
				},
				#endif
				
				#ifdef HBRS_MPL_ENABLE_ADDON_ELEMENTAL
				[](auto && a, auto economy) {
					return elemental::detail::pca_impl_Matrix{}(elemental::make_matrix(HBRS_MPL_FWD(a)), economy); 
				},
				#endif
				"SEQUENCE_TERMINATOR___REMOVED_BY_DROP_BACK"
			));
			
			auto results = hana::transform(funs, [&dataset, &economy](auto f) { return f(dataset, economy); });
			
			if constexpr(hana::length(results) >= 2u) {
				auto compare = [&dataset, &economy, &results](auto i) {
					using hbrs::mpl::select;
					
					auto j = i+hana::ushort_c<1>;
					auto const& pca_i = hana::at(results, i);
					auto const& pca_j = hana::at(results, j);
					
					auto sz_ = (*size)(dataset);
					auto m_ = (*m)(sz_);
					auto n_ = (*n)(sz_);
					
					BOOST_TEST_MESSAGE("comparing pca_coeff of impl nr " << i << " and " << j);
					auto const& pca_coeff_i = (*at)(pca_i, pca_coeff{});
					auto const& pca_coeff_j = (*at)(pca_j, pca_coeff{});
					if ((m_ < n_) && !economy) {
						auto rng = std::make_pair(make_matrix_index(0,0), make_matrix_size((int)n_, (int)m_-1 /* TODO: Why n*DOF instead of n*m? */));
						auto pca_coeff_i_mxn = (*select)(pca_coeff_i, rng);
						auto pca_coeff_j_mxn = (*select)(pca_coeff_j, rng);
						_BOOST_TEST_MMEQ(pca_coeff_i_mxn, pca_coeff_j_mxn, true);
					} else {
						_BOOST_TEST_MMEQ(pca_coeff_i, pca_coeff_j, true);
					}
					
					BOOST_TEST_MESSAGE("comparing pca_score of impl nr " << i << " and " << j);
					_BOOST_TEST_MMEQ((*at)(pca_i, pca_score{}),  (*at)(pca_j, pca_score{}), true);
					BOOST_TEST_MESSAGE("comparing pca_latent of impl nr " << i << " and " << j);
					_BOOST_TEST_VVEQ((*at)(pca_i, pca_latent{}), (*at)(pca_j, pca_latent{}), false);
					BOOST_TEST_MESSAGE("comparing pca_mean of impl nr " << i << " and " << j);
					_BOOST_TEST_VVEQ((*at)(pca_i, pca_mean{}),   (*at)(pca_j, pca_mean{}), false);
					BOOST_TEST_MESSAGE("comparing impl nr " << i << " and " << j << " done.");
				};
				
				hana::for_each(
					hana::make_range(
						hana::size_c<0>, 
						hana::length(results)-hana::size_c<2>
					),
					compare
				);
			}
			
			{
				auto rebuilds = hana::transform(results, [](auto && pca_result) {
					auto && coeff_ =  (*at) (pca_result, pca_coeff{});
					auto && score_ =  (*at) (pca_result, pca_score{});
// 					auto && latent_ = (*at) (pca_result, pca_latent{});
					auto && mean_ =   (*at) (pca_result, pca_mean{});
					
					auto centered = (*multiply)(score_, transpose(coeff_));
					return (*plus)(centered, expand(mean_, size(centered)));
				});
				
				hana::for_each(
					hana::make_range(hana::ushort_c<0>, hana::length(results)),
					[&dataset, &rebuilds](auto i) {
						BOOST_TEST_MESSAGE("comparing original and reconstructed dataset nr " << i);
						auto const& rebuild = hana::at(rebuilds, i);
						_BOOST_TEST_MMEQ(dataset, rebuild, false);
					}
				);
				BOOST_TEST_MESSAGE("comparing original and reconstructed datasets done.");
			}
		};
		
		++dataset_nr;
	});
	
}

BOOST_AUTO_TEST_SUITE_END()
