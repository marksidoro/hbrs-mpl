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

#define BOOST_TEST_MODULE fn_eig_hbrs_mpl_test
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
	#include <hbrs/mpl/detail/matlab_cxn.hpp>
#endif

#include <hbrs/mpl/detail/test.hpp>
#include <hbrs/mpl/detail/gather.hpp>
#include <hbrs/mpl/detail/not_supported.hpp>

#include <hbrs/mpl/dt/ctsav.hpp>
#include <hbrs/mpl/dt/ctsam.hpp>
#include <hbrs/mpl/dt/sm.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>
#include <hbrs/mpl/dt/eig_control.hpp>

#include <hbrs/mpl/fn/eig.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/minimum.hpp>
#include <hbrs/mpl/fn/select.hpp>
#include <hbrs/mpl/fn/multiply.hpp>
#include <hbrs/mpl/fn/diag.hpp>
#include <hbrs/mpl/fn/sort.hpp>
#include <hbrs/mpl/fn/absolute.hpp>

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

BOOST_AUTO_TEST_SUITE(fn_eig_hbrs_mpl_test)

using hbrs::mpl::detail::environment_fixture;
BOOST_TEST_GLOBAL_FIXTURE(environment_fixture);

struct less_norm_t {
#ifdef HBRS_MPL_ENABLE_MATLAB
	bool
	operator()(creal_T const& lhs, creal_T const& rhs) const {
		return (*this)(to_std_complex(lhs), to_std_complex(rhs));
	}
#endif

	template<typename T>
	bool
	operator()(std::complex<T> const& lhs, std::complex<T> const& rhs) const {
		using namespace hbrs::mpl;
		return (*absolute)(lhs) < (*absolute)(rhs);
	}
};

constexpr less_norm_t less_norm{};

BOOST_AUTO_TEST_CASE(eig_comparison,  * utf::tolerance(_TOL)) {
	using namespace hbrs::mpl;
	using hbrs::mpl::select;
	
	// tuple of square matrices
	static constexpr auto datasets = hana::make_tuple(
		make_sm(
			make_ctsav(detail::mat_a), make_matrix_size(hana::size_c<detail::mat_a_m>, hana::size_c<detail::mat_a_n>), row_major_c
		),
		make_ctsam(
			std::array<double, 8*8>{
				49, 64, 62, 23, 90, 60,  9, 24,
				44, 38, 59, 17, 98, 71, 26, 46,
				45, 81, 21, 23, 44, 22, 80, 96,
				31, 53, 30, 44, 11, 12,  3, 55,
				51, 35, 47, 31, 26, 30, 93, 52,
				51, 94, 23, 92, 41, 32, 73, 23,
				82, 88, 84, 43, 59, 42, 49, 49,
				79, 55, 19, 18, 26, 51, 58, 62
			},
			make_matrix_size(hana::size_c<8>, hana::size_c<8>),
			row_major_c
		)
	);
	
	static constexpr auto dimensions = hana::make_tuple(
		hana::to_tuple(hana::make_range(hana::size_c<0>, hana::length(datasets)))
	);
	
	static constexpr auto factories = hana::drop_back(hana::make_tuple(
		#ifdef HBRS_MPL_ENABLE_MATLAB
		[](auto && dataset) {
			return hana::make_tuple(
				detail::eig_impl_level0_ml_matrix{},
				make_ml_matrix(HBRS_MPL_FWD(dataset)),
				eig_control<>{}
			);
		},
		#endif
		#ifdef HBRS_MPL_ENABLE_ELEMENTAL
		[](auto && dataset) {
			return hana::make_tuple(
				detail::eig_impl_el_matrix{},
				make_el_matrix(HBRS_MPL_FWD(dataset)),
				eig_control<>{}
			);
		},
		[](auto && dataset) {
			static El::Grid grid{El::mpi::COMM_WORLD}; // grid is static because reference to grid is required by El::DistMatrix<...>
			return hana::make_tuple(
					detail::eig_impl_el_dist_matrix{},
					make_el_dist_matrix(grid, make_el_matrix(HBRS_MPL_FWD(dataset))),
					eig_control<>{}
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
				auto j = i+hana::ushort_c<1>;
				auto impl_idx_i = hana::at(supported_indices, i);
				auto impl_idx_j = hana::at(supported_indices, j);
				
				auto const& result_i = hana::at(results, i);
				auto const& result_j = hana::at(results, j);
				
				auto sorted_i = sort(result_i, less_norm);
				auto sorted_j = sort(result_j, less_norm);
				
				BOOST_TEST_PASSPOINT();
				
				/* TODO: Can signs and ordering of eigenvalues and eigenvectors differ depending on the algorithm used?
				 * "Since the eigenvalues may be complex, there is no fixed way to order them." [1]
				 * MATLAB Coder [2] and Elemental [3] use different algorithms for eig().
				 * Ref.:
				 * [1] http://www.netlib.org/utk/people/JackDongarra/etemplates/node49.html
				 * [2] src/hbrs/mpl/detail/matlab_cxn/impl/eig_level0.m
				 * [3] src/hbrs/mpl/fn/eig/impl/elemental.hpp
				 */
				BOOST_TEST_MESSAGE("Comparing eig_eigenvalues of impl nr " << impl_idx_i << " and " << impl_idx_j);
				auto const& eig_eigenvalues_i = (*at)(sorted_i, eig_eigenvalues{});
				auto const& eig_eigenvalues_j = (*at)(sorted_j, eig_eigenvalues{});
				HBRS_MPL_TEST_VVEQ(eig_eigenvalues_i, eig_eigenvalues_j, true);
				
				BOOST_TEST_MESSAGE("Comparing eig_eigenvectors of impl nr " << impl_idx_i << " and " << impl_idx_j);
				auto const& eig_eigenvectors_i = (*at)(sorted_i, eig_eigenvectors{});
				auto const& eig_eigenvectors_j = (*at)(sorted_j, eig_eigenvectors{});
				
				//Compare 2-norm/absolute of complex numbers only
				HBRS_MPL_TEST_MMEQ(eig_eigenvectors_i, eig_eigenvectors_j, true);
				
				BOOST_TEST_MESSAGE("Comparing impl nr " << impl_idx_i << " and " << impl_idx_j << " done.");
				
				BOOST_TEST_PASSPOINT();
			};
			
			hana::for_each(hana::drop_back(results_indices), compare);
		}
		
		BOOST_TEST_PASSPOINT();
		
		// Test A*V = V*D where V is a matrix whose columns are the eigenvectors 
		// of A and D is a diagonal matrix with eigenvalues of A
		
		hana::for_each(
			results_indices,
			[&](auto i) {
				auto impl_idx = hana::at(supported_indices, i);
				BOOST_TEST_MESSAGE("Comparing A*V and V*D of impl nr " << impl_idx);
				
				auto testcase = hana::at(testcases, impl_idx);
				auto A = hana::at(testcase, hana::size_c<1>);
				auto eig_result = hana::at(results, i);
				auto D = (*at)(eig_result, eig_eigenvalues{});
				auto V = (*at)(eig_result, eig_eigenvectors{});
				
				auto AV = (*multiply)(A, V);
				auto VD = (*multiply)(V, diag(D));
				
				//Compare 2-norm/absolute of complex numbers only
				HBRS_MPL_TEST_MMEQ(AV, VD, true);
			}
		);
		BOOST_TEST_MESSAGE("Comparing A*V and V*D done.");
		
		//TODO: Test more properties of eigenvalues and eigenvectors?
		
	});
	
}

BOOST_AUTO_TEST_SUITE_END()
