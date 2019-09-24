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
#include <boost/hana/min.hpp>
#include <boost/hana/greater_equal.hpp>
#include <boost/hana/less_equal.hpp>
#include <boost/hana/length.hpp>
#include <boost/hana/not.hpp>
#include <boost/hana/cartesian_product.hpp>

#include <hbrs/mpl/detail/test.hpp>
#include <hbrs/mpl/detail/not_supported.hpp>

namespace utf = boost::unit_test;
namespace tt = boost::test_tools;

BOOST_AUTO_TEST_SUITE(pca_filter_test)

using hbrs::mpl::detail::environment_fixture;
BOOST_TEST_GLOBAL_FIXTURE(environment_fixture);

BOOST_AUTO_TEST_CASE(pca_filter_comparison,  * utf::tolerance(0.000000001)) {
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
		hana::make_tuple(hana::true_c, hana::false_c) /* keep */,
		hana::make_tuple(hana::true_c, hana::false_c) /* economy */,
		hana::make_tuple(hana::true_c, hana::false_c) /* center */,
		hana::make_tuple(hana::true_c, hana::false_c) /* normalize */
	);
	
	hana::for_each(hana::cartesian_product(dimensions), [](auto const& cfg) {
		auto const& dataset_nr = hana::at_c<0>(cfg);
		auto const& keep = hana::at_c<1>(cfg);
		auto const& economy = hana::at_c<2>(cfg);
		auto const& center = hana::at_c<3>(cfg);
		auto const& normalize = hana::at_c<3>(cfg);
		
		BOOST_TEST_MESSAGE("dataset_nr=" << dataset_nr);
		BOOST_TEST_MESSAGE("keep=" << (keep ? "true" : "false"));
		BOOST_TEST_MESSAGE("economy=" << (economy ? "true" : "false"));
		BOOST_TEST_MESSAGE("center=" << (center? "true" : "false"));
		BOOST_TEST_MESSAGE("normalize=" << (normalize? "true" : "false"));
		
		auto const& dataset = hana::at(datasets, dataset_nr);
		auto sz_ = (*size)(dataset);
		auto m_ = (*m)(sz_);
		auto n_ = (*n)(sz_);
		
		auto funs = hana::drop_back(hana::make_tuple(
			#ifdef HBRS_MPL_ENABLE_MATLAB
			[](auto && a, auto keep, auto economy, auto center, auto normalize) {
				return detail::pca_filter_impl_ml_matrix{}(hbrs::mpl::make_ml_matrix(HBRS_MPL_FWD(a)), keep, {economy, center, normalize});
			},
			#endif
			
			#ifdef HBRS_MPL_ENABLE_ELEMENTAL
			[](auto && a, auto keep, auto economy, auto center, auto normalize) {
				return detail::pca_filter_impl_el_matrix{}(hbrs::mpl::make_el_matrix(HBRS_MPL_FWD(a)), keep, {economy, center, normalize});
			},
			[](auto && a, auto keep, auto economy, auto center, auto normalize) {
				static El::Grid grid{El::mpi::COMM_WORLD}; // grid is static because reference to grid is required by El::DistMatrix<...>
				auto a_sz = (*size)(a);
				if constexpr(
					!hana::value(economy) && (hana::value(a_sz.m()) <= hana::value(a_sz.n()))
					/* because this will do a zero svd which equals complete which is not supported by elemental */
				) {
					return detail::not_supported{};
				} else {
					return detail::pca_filter_impl_el_matrix{}(
						hbrs::mpl::make_el_dist_matrix(
							grid,
							hbrs::mpl::make_el_matrix(HBRS_MPL_FWD(a))
						),
						keep,
						{economy, center, normalize}
					);
				}
			},
			#endif
			"SEQUENCE_TERMINATOR___REMOVED_BY_DROP_BACK"
		));
		
		auto DOF = m_.value - (center ? hana::size_c<1> : hana::size_c<0>);
		std::vector<bool> filter((DOF < n_.value && economy) ? DOF : std::min(m_.value, n_.value), keep);
		
		int fun_nr = 0;
		auto results = hana::transform(funs, [&](auto f) {
			BOOST_TEST_MESSAGE("calling impl nr " << fun_nr);
			++fun_nr;
			return f(dataset, filter, economy, center, normalize); }
		);
		
		if constexpr(hana::length(results) >= 2u) {
			auto compare = [&results](auto i) {
				auto j = i+hana::ushort_c<1>;
				auto const& pca_filter_i = hana::at(results, i);
				auto const& pca_filter_j = hana::at(results, j);
				
				//TODO: Take ++j if j not supported!
				if constexpr(is_supported(pca_filter_i) && is_supported(pca_filter_j)) {
					BOOST_TEST_MESSAGE("comparing data of impl nr " << i << " and " << j);
					HBRS_MPL_TEST_MMEQ((*at)(pca_filter_i, pca_filter_data{}),  (*at)(pca_filter_j, pca_filter_data{}), false);
					BOOST_TEST_MESSAGE("comparing pca_latent of impl nr " << i << " and " << j);
					HBRS_MPL_TEST_VVEQ((*at)(pca_filter_i, pca_filter_latent{}), (*at)(pca_filter_j, pca_filter_latent{}), false);
					BOOST_TEST_MESSAGE("comparing impl nr " << i << " and " << j << " done.");
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
			hana::for_each(
				hana::make_range(hana::ushort_c<0>, hana::length(results)),
				[&](auto i) {
					auto const& result = hana::at(results, i);
					
					if constexpr(is_not_supported(result)) {
						BOOST_TEST_MESSAGE("skip comparison of unsupported dataset for impl nr " << i);
						return detail::not_supported{};
					} else {
						BOOST_TEST_MESSAGE("comparing original and reconstructed dataset from impl nr " << i);
						auto && data_   = (*at)(result, pca_filter_data{});
						auto && latent_ = (*at)(result, pca_filter_latent{});
						
						if (keep) {
							HBRS_MPL_TEST_MMEQ(dataset, data_, false);
						} else {
							if (center) {
								auto mean_ = (*expand)(mean(columns(dataset)), size(dataset));
								HBRS_MPL_TEST_MMEQ(mean_, data_, false);
							} else {
								//TODO: Compare if all values of data_ are zero!
								BOOST_TEST_MESSAGE("... not yet implemented");
							}
						}
						
						auto DOF = m_ - (center? 1u : 0u);
						BOOST_TEST((*equal)(size(latent_), (DOF<n_ && !economy) ? n_ : hana::min(DOF,n_)));
					}
				}
			);
			BOOST_TEST_MESSAGE("comparing original and reconstructed datasets done.");
		}
	});
	
}

BOOST_AUTO_TEST_SUITE_END()
