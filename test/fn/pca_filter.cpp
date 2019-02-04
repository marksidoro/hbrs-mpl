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

#define BOOST_TEST_MODULE pca_filter_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/fn/pca_filter.hpp>
#include <hbrs/mpl/detail/test.hpp>
#include <hbrs/mpl/dt/ctsav.hpp>
#include <hbrs/mpl/dt/sm.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/plus.hpp>
#include <hbrs/mpl/fn/multiply.hpp>
#include <hbrs/mpl/fn/transpose.hpp>
#include <hbrs/mpl/fn/expand.hpp>
#include <hbrs/mpl/fn/columns.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/mean.hpp>
#ifdef HBRS_MPL_ENABLE_ADDON_ELEMENTAL
	#include <elemental/dt/matrix.hpp>
	#include <elemental/dt/dist_matrix.hpp>
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
#include <boost/hana/min.hpp>

#include "../data.hpp"

namespace utf = boost::unit_test;
namespace tt = boost::test_tools;

BOOST_AUTO_TEST_SUITE(pca_filter_test)

using hbrs::mpl::detail::environment_fixture;
BOOST_TEST_GLOBAL_FIXTURE(environment_fixture);

BOOST_AUTO_TEST_CASE(pca_filter_comparison,  * utf::tolerance(0.000000001)) {
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
		for(bool keep : { true, false }) {
			BOOST_TEST_MESSAGE("keep=" << (keep ? "true" : "false"));
			
			auto sz_ = (*size)(dataset);
			auto m_ = (*m)(sz_);
			auto n_ = (*n)(sz_);
			
			auto funs = hana::drop_back(hana::make_tuple(
				#ifdef HBRS_MPL_ENABLE_ADDON_MATLAB
				[](auto && a, auto keep) {
					return matlab::detail::pca_filter_impl{}(matlab::make_matrix(HBRS_MPL_FWD(a)), keep);
				},
				#endif
				
				#ifdef HBRS_MPL_ENABLE_ADDON_ELEMENTAL
				[](auto && a, auto keep) {
					return elemental::detail::pca_filter_impl_Matrix{}(elemental::make_matrix(HBRS_MPL_FWD(a)), keep);
				},
				[](auto && a, auto keep) {
					static El::Grid grid{El::mpi::COMM_WORLD}; // grid is static because reference to grid is required by El::DistMatrix<...>
					return elemental::detail::pca_filter_impl_DistMatrix{}(
						elemental::make_dist_matrix(
							grid,
							elemental::make_matrix(HBRS_MPL_FWD(a))
						),
						keep
					);
				},
				#endif
				"SEQUENCE_TERMINATOR___REMOVED_BY_DROP_BACK"
			));
			
			auto results = hana::transform(funs, 
				[&dataset, &keep, &m_, &n_](auto f) {
					return f(dataset, std::vector<bool>(m_.value-1<n_.value? m_.value-1 : std::min(m_.value, n_.value), keep));
				}
			);
			
			if constexpr(hana::length(results) >= 2u) {
				auto compare = [&results](auto i) {
					auto j = i+hana::ushort_c<1>;
					auto const& pca_filter_i = hana::at(results, i);
					auto const& pca_filter_j = hana::at(results, j);
					
					BOOST_TEST_MESSAGE("comparing data of impl nr " << i << " and " << j);
					HBRS_MPL_TEST_MMEQ((*at)(pca_filter_i, pca_filter_data{}),  (*at)(pca_filter_j, pca_filter_data{}), false);
					BOOST_TEST_MESSAGE("comparing pca_latent of impl nr " << i << " and " << j);
					HBRS_MPL_TEST_VVEQ((*at)(pca_filter_i, pca_filter_latent{}), (*at)(pca_filter_j, pca_filter_latent{}), false);
					BOOST_TEST_MESSAGE("comparing impl nr " << i << " and " << j << " done.");
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
				hana::for_each(
					hana::make_range(hana::ushort_c<0>, hana::length(results)),
					[&dataset, &results, &keep, &m_, &n_](auto i) {
						
						BOOST_TEST_MESSAGE("comparing original and reconstructed dataset from impl nr " << i);
						auto const& result = hana::at(results, i);
						
						auto && data_   = (*at)(result, pca_filter_data{});
						auto && latent_ = (*at)(result, pca_filter_latent{});
						
						if (keep) {
							HBRS_MPL_TEST_MMEQ(dataset, data_, false);
						} else {
							auto mean_ = (*expand)(mean(columns(dataset)), size(dataset));
							HBRS_MPL_TEST_MMEQ(mean_, data_, false);
						}
						
						BOOST_TEST((*equal)(size(latent_), (m_-1u)<n_ ? m_-1u : hana::min(m_,n_)));
					}
				);
				BOOST_TEST_MESSAGE("comparing original and reconstructed datasets done.");
			}
		};
		
		++dataset_nr;
	});
	
}

BOOST_AUTO_TEST_SUITE_END()
