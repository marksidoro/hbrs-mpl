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

#define BOOST_TEST_MODULE fn_mldivide_hbrs_mpl_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/config.hpp>

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
	#include <hbrs/mpl/dt/el_matrix.hpp>
	#include <hbrs/mpl/dt/el_dist_matrix.hpp>
	#include <hbrs/mpl/dt/el_vector.hpp>
	#include <hbrs/mpl/dt/el_dist_vector.hpp>
#endif
#ifdef HBRS_MPL_ENABLE_MATLAB
	#include <hbrs/mpl/dt/ml_matrix.hpp>
	#include <hbrs/mpl/dt/ml_vector.hpp>
#endif

#include <hbrs/mpl/detail/test.hpp>
#include <hbrs/mpl/detail/gather.hpp>
#include <hbrs/mpl/detail/not_supported.hpp>

#include <hbrs/mpl/dt/ctsam.hpp>
#include <hbrs/mpl/dt/ctsav.hpp>
#include <hbrs/mpl/dt/scv.hpp>
#include <hbrs/mpl/dt/sm.hpp>
#include <hbrs/mpl/dt/storage_order.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>

#include <hbrs/mpl/fn/mldivide.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/minimum.hpp>
#include <hbrs/mpl/fn/select.hpp>
#include <hbrs/mpl/fn/multiply.hpp>
#include <hbrs/mpl/fn/diag.hpp>

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
#include <boost/hana/remove_at.hpp>

#include <array>

namespace utf = boost::unit_test;
namespace tt = boost::test_tools;

#define _TOL 0.000000001

BOOST_AUTO_TEST_SUITE(fn_mldivide_hbrs_mpl_test)

using hbrs::mpl::detail::environment_fixture;
BOOST_TEST_GLOBAL_FIXTURE(environment_fixture);

BOOST_AUTO_TEST_CASE(mldivide_comparison_Ax_b,  * utf::tolerance(_TOL)) {
	using namespace hbrs::mpl;
	using hbrs::mpl::select;
	
	// tuple of triplet A*x=b
	static constexpr auto datasets = hana::make_tuple(
		hana::make_tuple(
			make_ctsam(
				std::array<double, 4*4>{
					3, 2, 9, 9,
					8, 3, 6, 6,
					4, 1, 5, 4,
					9, 1, 1, 5
				},
				make_matrix_size(hana::size_c<4>, hana::size_c<4>),
				row_major_c
			),
			make_scv(std::array<double, 4>{
				2,
				3,
				4,
				5
			}),
			make_scv(std::array<double, 4>{
				93,
				79,
				51,
				50
			})
		),
		hana::make_tuple(
			make_ctsam(
				std::array<double, 6*3>{
					2 , 5, 0,
					5 , 6, 9,
					10, 7, 9,
					5 , 4, 8,
					5 , 4, 1,
					2 ,10, 3
				},
				make_matrix_size(hana::size_c<6>, hana::size_c<3>),
				row_major_c
			),
			make_scv(std::array<double, 3>{
				5,
				1,
				7,
			}),
			make_scv(std::array<double, 6>{
				15,
				94,
				120,
				85,
				36,
				41
			})
		),
		hana::make_tuple(
			make_ctsam(
				std::array<double, 3*6>{
					9, 2, 10, 0, 7, 8,
					6, 2,  7, 1, 1, 5,
					6, 9,  5, 5, 7, 4
				},
				make_matrix_size(hana::size_c<3>, hana::size_c<6>),
				row_major_c
			),
			/* Just one out of infinite solutions */
// 			make_scv(std::array<double, 6>{
// 				0,
// 				2.849849849849845,
// 				13.789789789789792,
// 				0,
// 				6.771771771771776,
// 				0
// 			}),
			/* Minimum norm least-squares solution */
			make_scv(std::array<double, 6>{
				5.567537795335410,
				2.076160449745499,
				5.880878219250929,
				0.401884068981235,
				5.579996961179068,
				4.858884752715945
			}),
			make_scv(std::array<double, 3>{
				191,
				109,
				142
			})
		),
		hana::make_tuple(
			make_sm(
				make_ctsav(detail::mat_a), make_matrix_size(hana::size_c<detail::mat_a_m>, hana::size_c<detail::mat_a_n>), row_major_c
			),
			make_scv(std::array<double, 3>{
				0.377959183673469,
				0.001959183673469,
				-0.108897959183673
			}),
			make_scv(std::array<double, 3>{
				4,
				10,
				3
			})
		),
		hana::make_tuple(
			make_sm(
				make_ctsav(detail::mat_g), make_matrix_size(hana::size_c<detail::mat_g_m>, hana::size_c<detail::mat_g_n>), row_major_c
			),
			make_scv(std::array<double, 4>{
			   0.798278717896806,
				-0.166050119364084,
				1.129748343375009,
				0.042856012126022
			}),
			make_scv(std::array<double, 13>{
				7,
				18,
				3,
				21,
				3,
				15,
				9,
				20,
				13,
				12,
				22,
				9,
				11
			})
		),
		hana::make_tuple(
			make_sm(
				make_ctsav(detail::mat_j), make_matrix_size(hana::size_c<detail::mat_j_m>, hana::size_c<detail::mat_j_n>), row_major_c
			),
			make_scv(std::array<double, 13>{
				-0.570377301256899,
				-0.291004428526117,
				0.070994228538842,
				0.253928901522805,
				-0.783503338279968,
				0.163026564614126,
				-0.130615484210031,
				0.547451033421538,
				0.025082224434169,
				1.027657436905407,
				0.591725030436288,
				0.042717644626068,
				-0.244747206214567
			}),
			make_scv(std::array<double, 4>{
				16,
				24,
				18,
				10
			})
		)
	);
	
	static constexpr auto dimensions = hana::make_tuple(
		hana::to_tuple(hana::make_range(hana::size_c<0>, hana::length(datasets)))
	);
	
	static constexpr auto factories = hana::drop_back(hana::make_tuple(
		#ifdef HBRS_MPL_ENABLE_MATLAB
		[](auto && dataset) {
			auto A = hana::at(dataset, hana::size_c<0>);
			auto x = hana::at(dataset, hana::size_c<1>);
			auto b = hana::at(dataset, hana::size_c<2>);
			return hana::make_tuple(
				detail::mldivide_impl_ml_matrix_ml_vector{},
				make_ml_matrix(std::move(A)),
				make_ml_column_vector(std::move(x)),
				make_ml_column_vector(std::move(b))
			);
		},
		#endif
		
		#ifdef HBRS_MPL_ENABLE_ELEMENTAL
		[](auto && dataset) {
			auto A = hana::at(dataset, hana::size_c<0>);
			auto x = hana::at(dataset, hana::size_c<1>);
			auto b = hana::at(dataset, hana::size_c<2>);
			return hana::make_tuple(
				detail::mldivide_impl_el_matrix_el_vector{},
				make_el_matrix(std::move(A)),
				make_el_column_vector(std::move(x)),
				make_el_column_vector(std::move(b))
			);
		},
		[](auto && dataset) {
			static El::Grid grid{El::mpi::COMM_WORLD}; // grid is static because reference to grid is required by El::DistMatrix<...>
			auto A = hana::at(dataset, hana::size_c<0>);
			auto x = hana::at(dataset, hana::size_c<1>);
			auto b = hana::at(dataset, hana::size_c<2>);
			return hana::make_tuple(
				detail::mldivide_impl_el_dist_matrix_el_dist_vector{},
				make_el_dist_matrix(grid, make_el_matrix(std::move(A))),
				make_el_dist_column_vector(grid, make_el_column_vector(std::move(x))),
				make_el_dist_column_vector(grid, make_el_column_vector(std::move(b)))
			);
		},
		#endif
		"SEQUENCE_TERMINATOR___REMOVED_BY_DROP_BACK"
	));
	
	hana::for_each(hana::cartesian_product(dimensions), [](auto const& cfg) {
		auto const& dataset_nr = hana::at_c<0>(cfg);
		BOOST_TEST_MESSAGE("dataset_nr=" << dataset_nr);
		auto const& dataset = hana::at(datasets, dataset_nr);
		
		auto testcases = hana::transform(
			factories,
			[&](auto factory) { return factory(dataset); }
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
					hana::drop_front(hana::remove_at(testcase, hana::size_c<2>)),
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
				auto j = i+hana::ushort_c<1>;
				auto impl_idx_i = hana::at(supported_indices, i);
				auto impl_idx_j = hana::at(supported_indices, j);
				
				auto const& result_i = hana::at(results, i);
				auto const& result_j = hana::at(results, j);
				
				BOOST_TEST_PASSPOINT();
				
				BOOST_TEST_MESSAGE("comparing results of impl nr " << impl_idx_i << " and " << impl_idx_j);
				
				//NOTE: Results from MATLAB and Elemental differ, see src/hbrs/mpl/fn/mldivide/impl/elemental.hpp!
				HBRS_MPL_TEST_VVEQ(result_i, result_j, false);
				
				BOOST_TEST_MESSAGE("comparing impl nr " << impl_idx_i << " and " << impl_idx_j << " done.");
				
				BOOST_TEST_PASSPOINT();
			};
			
			hana::for_each(hana::drop_back(results_indices), compare);
		}
		
		BOOST_TEST_PASSPOINT();
		
		// Test A*x = b
		hana::for_each(
			results_indices,
			[&](auto i) {
				auto impl_idx = hana::at(supported_indices, i);
				BOOST_TEST_MESSAGE("Comparing A*x and b computed by impl nr " << impl_idx);
				
				auto testcase = hana::at(testcases, impl_idx);
				auto A = hana::at(testcase, hana::size_c<1>);
				auto x = hana::at(testcase, hana::size_c<2>);
				auto b = hana::at(testcase, hana::size_c<3>);
				auto x_ = hana::at(results, i);
				
				auto Ax = (*multiply)(A, x);
				auto Ax_ = (*multiply)(A, x_);
				
				BOOST_TEST_MESSAGE("A*x == A*x_?");
				HBRS_MPL_TEST_VVEQ(Ax, Ax_, false);
				// TODO: A*x == A*x_, but A*x != b and A*x_ != b, but why?
				BOOST_TEST_MESSAGE("A*x == b?");
				HBRS_MPL_TEST_VVEQ(Ax, b, false);
				BOOST_TEST_MESSAGE("A*x_ == b?");
				HBRS_MPL_TEST_VVEQ(Ax_, b, false);
				BOOST_TEST_MESSAGE("x == x_?");
				//NOTE: Results from MATLAB and Elemental differ, see src/hbrs/mpl/fn/mldivide/impl/elemental.hpp!
				HBRS_MPL_TEST_VVEQ(x, x_, false);
			}
		);
		BOOST_TEST_MESSAGE("comparing A*x and b done.");
	});
	
}

BOOST_AUTO_TEST_SUITE_END()
