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

#define BOOST_TEST_MODULE fn_dmd_hbrs_mpl_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/config.hpp>

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
	#include <hbrs/mpl/dt/el_matrix.hpp>
	#include <hbrs/mpl/dt/el_dist_matrix.hpp>
#endif
#ifdef HBRS_MPL_ENABLE_MATLAB
	#include <hbrs/mpl/dt/ml_matrix.hpp>
#endif

#include <hbrs/mpl/detail/test.hpp>
#include <hbrs/mpl/detail/gather.hpp>
#include <hbrs/mpl/detail/not_supported.hpp>

#include <hbrs/mpl/dt/ctsav.hpp>
#include <hbrs/mpl/dt/sm.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>
#include <hbrs/mpl/dt/dmd_control.hpp>

#include <hbrs/mpl/fn/dmd.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/minimum.hpp>
#include <hbrs/mpl/fn/select.hpp>

#include <boost/hana/filter.hpp>
#include <boost/hana/zip.hpp>
#include <boost/hana/first.hpp>
#include <boost/hana/second.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/at.hpp>
#include <boost/hana/plus.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/for_each.hpp>
#include <boost/hana/transform.hpp>
#include <boost/hana/cartesian_product.hpp>
#include <boost/hana/drop_back.hpp>
#include <boost/hana/drop_front.hpp>
#include <boost/hana/front.hpp>
#include <boost/hana/back.hpp>
#include <boost/hana/unpack.hpp>
#include <boost/hana/greater_equal.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/length.hpp>

namespace utf = boost::unit_test;
namespace tt = boost::test_tools;

#define _TOL 0.000000001

BOOST_AUTO_TEST_SUITE(fn_dmd_hbrs_mpl_test)

using hbrs::mpl::detail::environment_fixture;
BOOST_TEST_GLOBAL_FIXTURE(environment_fixture);

BOOST_AUTO_TEST_CASE(dmd_comparison,  * utf::tolerance(_TOL)) {
	using namespace hbrs::mpl;
	using hbrs::mpl::select;
	
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
		hana::to_tuple(hana::make_range(hana::size_c<0>, hana::length(datasets)))
	);
	
	static constexpr auto factories = hana::drop_back(hana::make_tuple(
		#ifdef HBRS_MPL_ENABLE_MATLAB
		[](auto && dataset, auto && target_rank) {
			auto x_ = make_ml_matrix(HBRS_MPL_FWD(dataset));
			auto sz_ = (*size)(x_);
			auto m_ = (*m)(sz_);
			auto n_ = (*n)(sz_);
		
			auto x1_ = (*select)(x_, std::make_pair(matrix_index<int,int>{0,0}, matrix_size<int,int>{m_, n_-1}));
			auto x2_ = (*select)(x_, std::make_pair(matrix_index<int,int>{0,1}, matrix_size<int,int>{m_, n_-1}));
			return hana::make_tuple(
				detail::dmd_impl_level1_ml_matrix{},
				std::move(x1_),
				std::move(x2_),
				dmd_control<int>{ (int)target_rank}
			);
		},
		#endif
		#ifdef HBRS_MPL_ENABLE_ELEMENTAL
		[](auto && dataset, auto && target_rank) {
			auto x_ = make_el_matrix(HBRS_MPL_FWD(dataset));
			auto sz_ = (*size)(x_);
			auto m_ = (*m)(sz_);
			auto n_ = (*n)(sz_);
		
			auto x1_ = (*select)(x_, std::make_pair(matrix_index<El::Int,El::Int>{0,0}, matrix_size<El::Int,El::Int>{m_, n_-1}));
			auto x2_ = (*select)(x_, std::make_pair(matrix_index<El::Int,El::Int>{0,1}, matrix_size<El::Int,El::Int>{m_, n_-1}));
			
			return hana::make_tuple(
				detail::dmd_impl_el_matrix{},
				std::move(x1_),
				std::move(x2_),
				dmd_control<El::Int>{(El::Int)target_rank}
			);
		},
		[](auto && dataset, auto && target_rank) {
			static El::Grid grid{El::mpi::COMM_WORLD}; // grid is static because reference to grid is required by El::DistMatrix<...>
			auto x_ = make_el_dist_matrix(grid, make_el_matrix(HBRS_MPL_FWD(dataset)));
			auto sz_ = (*size)(x_);
			auto m_ = (*m)(sz_);
			auto n_ = (*n)(sz_);
		
			auto x1_ = (*select)(x_, std::make_pair(matrix_index<El::Int,El::Int>{0,0}, matrix_size<El::Int,El::Int>{m_, n_-1}));
			auto x2_ = (*select)(x_, std::make_pair(matrix_index<El::Int,El::Int>{0,1}, matrix_size<El::Int,El::Int>{m_, n_-1}));

			return hana::make_tuple(
				detail::dmd_impl_el_dist_matrix{},
				std::move(x1_),
				std::move(x2_),
				dmd_control<El::Int>{(El::Int)target_rank}
			);
		},
		#endif
		"SEQUENCE_TERMINATOR___REMOVED_BY_DROP_BACK"
	));
	
	hana::for_each(hana::cartesian_product(dimensions), [](auto const& cfg) {
		auto const& dataset_nr = hana::at_c<0>(cfg);
		auto const& dataset = hana::at(datasets, dataset_nr);
		auto sz_ = (*size)(dataset);
		auto m_ = (*m)(sz_);
		auto n_ = (*n)(sz_);
		
		auto max_rank = (*minimum)(m_, n_);
		auto target_ranks = hana::to_tuple(hana::make_range(hana::size_c<1>, max_rank + hana::size_c<1>));
		
		hana::for_each(target_ranks, [&](auto const& target_rank) {
			BOOST_TEST_MESSAGE("dataset_nr=" << dataset_nr);
			BOOST_TEST_MESSAGE("target_rank=" << target_rank);
			
			auto testcases = hana::transform(
				factories,
				[&](auto factory) { return factory(dataset, target_rank); }
			);
			
			auto supported_indices = hana::transform(
				hana::filter(
					hana::zip(
						hana::transform(testcases, detail::is_supported), // tuple of hana::true_ and hana::false_
						hana::to_tuple(hana::make_range(hana::size_c<0>,hana::length(testcases))) // indices of testcases
					),
					hana::front
				),
				hana::back
			);
			
			auto results = hana::transform(
				supported_indices,
				[&](auto i) {
					BOOST_TEST_MESSAGE("Running impl nr " << i);
					auto testcase = hana::at(testcases, i);
					return hana::unpack(
						hana::drop_front(testcase),
						hana::front(testcase)
					);
				}
			);
			
			auto results_indices = hana::to_tuple(hana::make_range(
				hana::size_c<0>,
				hana::length(results)
			));
			
			BOOST_TEST_MESSAGE("All runs finished.");
			
			BOOST_TEST_PASSPOINT();
			
			if constexpr(hana::length(results) >= 2u) {
				auto compare = [&](auto i) {
					using hbrs::mpl::select;
					
					auto j = i+hana::ushort_c<1>;
					auto impl_idx_i = hana::at(supported_indices, i);
					auto impl_idx_j = hana::at(supported_indices, j);
					
					auto const& result_i = hana::at(results, i);
					auto const& result_j = hana::at(results, j);
					
					BOOST_TEST_PASSPOINT();
					
					BOOST_TEST_MESSAGE("comparing dmd_eigenvalues of impl nr " << impl_idx_i << " and " << impl_idx_j);
					auto const& dmd_eigenvalues_i = (*at)(result_i, dmd_eigenvalues{});
					auto const& dmd_eigenvalues_j = (*at)(result_j, dmd_eigenvalues{});
					HBRS_MPL_TEST_VVEQ(dmd_eigenvalues_i, dmd_eigenvalues_j, true);
					
					BOOST_TEST_MESSAGE("comparing dmd_modes of impl nr " << impl_idx_i << " and " << impl_idx_j);
					auto const& dmd_modes_i = (*at)(result_i, dmd_modes{});
					auto const& dmd_modes_j = (*at)(result_j, dmd_modes{});
					HBRS_MPL_TEST_MMEQ(dmd_modes_i, dmd_modes_j, true);
					
					BOOST_TEST_MESSAGE("comparing dmd_coefficients of impl nr " << impl_idx_i << " and " << impl_idx_j);
					auto const& dmd_coefficients_i = (*at)(result_i, dmd_coefficients{});
					auto const& dmd_coefficients_j = (*at)(result_j, dmd_coefficients{});
					HBRS_MPL_TEST_VVEQ(dmd_coefficients_i, dmd_coefficients_j, true);
					
					BOOST_TEST_MESSAGE("comparing impl nr " << impl_idx_i << " and " << impl_idx_j << " done.");
					
					BOOST_TEST_PASSPOINT();
					
				};
				
				hana::for_each(hana::drop_back(results_indices), compare);
			}
			
			BOOST_TEST_PASSPOINT();
			
			{
				//TODO: Properly test eigenvalues, modes and coefficients
			}
			
			BOOST_TEST_PASSPOINT();
		});
	});
	
}

BOOST_AUTO_TEST_SUITE_END()
