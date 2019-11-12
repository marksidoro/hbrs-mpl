/* Copyright (c) 2019 Abdullah Güntepe, <abdullah@guentepe.com>
 * Copyright (c) 2019 Jakob Meng, <jakobmeng@web.de>
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

#define BOOST_TEST_MODULE fn_bidiag_hbrs_mpl_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#ifdef HBRS_MPL_ENABLE_MATLAB
	#include <hbrs/mpl/dt/ml_matrix.hpp>
#endif

#include <hbrs/mpl/detail/test.hpp>
#include <hbrs/mpl/detail/gather.hpp>
#include <hbrs/mpl/detail/not_supported.hpp>

#include <hbrs/mpl/dt/rtsam.hpp>
#include <hbrs/mpl/dt/storage_order.hpp>
#include <hbrs/mpl/dt/ctsav.hpp>
#include <hbrs/mpl/dt/sm.hpp>
#include <hbrs/mpl/dt/range.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>
#include <hbrs/mpl/dt/decompose_mode.hpp>
#include <hbrs/mpl/dt/bidiag_control.hpp>

#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/bidiag.hpp>
#include <hbrs/mpl/fn/transpose.hpp>
#include <hbrs/mpl/fn/select.hpp>
#include <hbrs/mpl/fn/multiply.hpp>

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

BOOST_AUTO_TEST_SUITE(fn_bidiag_hbrs_mpl_test)

using hbrs::mpl::detail::environment_fixture;
BOOST_TEST_GLOBAL_FIXTURE(environment_fixture);

//TODO: Refactor like fn_variance_hbrs_mpl_test
BOOST_AUTO_TEST_CASE(bidiag_comparison, * utf::tolerance(_TOL)) {
	using namespace hbrs::mpl;
	using hbrs::mpl::decompose_mode;
	
	static constexpr auto datasets = hana::make_tuple(
		make_sm(
			make_ctsav(detail::mat_a), make_matrix_size(hana::size_c<detail::mat_a_m>, hana::size_c<detail::mat_a_n>), row_major_c
		),
		make_sm(
			make_ctsav(detail::mat_g), make_matrix_size(hana::size_c<detail::mat_g_m>, hana::size_c<detail::mat_g_n>), row_major_c
		),
		make_sm(
			make_ctsav(detail::mat_j), make_matrix_size(hana::size_c<detail::mat_j_m>, hana::size_c<detail::mat_j_n>), row_major_c
		),
		make_sm(
			make_ctsav(detail::mat_k), make_matrix_size(hana::size_c<detail::mat_k_m>, hana::size_c<detail::mat_k_n>), row_major_c
		),
		make_sm(
			make_ctsav(detail::mat_l), make_matrix_size(hana::size_c<detail::mat_l_m>, hana::size_c<detail::mat_l_n>), row_major_c
		),
		make_sm(
			make_ctsav(detail::mat_m), make_matrix_size(hana::size_c<detail::mat_m_m>, hana::size_c<detail::mat_m_n>), row_major_c
		),
		make_sm(
			make_ctsav(detail::mat_n), make_matrix_size(hana::size_c<detail::mat_n_m>, hana::size_c<detail::mat_n_n>), row_major_c
		),
		make_sm(
			make_ctsav(detail::mat_o), make_matrix_size(hana::size_c<detail::mat_o_m>, hana::size_c<detail::mat_o_n>), row_major_c
		),
		make_sm(
			make_ctsav(detail::mat_p), make_matrix_size(hana::size_c<detail::mat_p_m>, hana::size_c<detail::mat_p_n>), row_major_c
		)
	);
	
	static constexpr auto dimensions = hana::make_tuple(
		hana::to_tuple(hana::make_range(hana::size_c<0>, hana::length(datasets))),
		hana::make_tuple(complete_decompose_c, economy_decompose_c, zero_decompose_c)
	);
	
	static constexpr auto factories = hana::drop_back(hana::make_tuple(
		#ifdef HBRS_MPL_ENABLE_MATLAB
		[](auto && a, auto mode) {
			
			auto sz_ = (*size)(a);
			auto m_ = (*m)(sz_);
			auto n_ = (*n)(sz_);
			
			if constexpr(
				(mode != decompose_mode::complete) || (hana::value(m_) < hana::value(n_))
			) {
				return detail::not_supported{};
			} else {
			
				return hana::make_tuple(
					detail::bidiag_impl_level0_ml_matrix{},
					make_ml_matrix(HBRS_MPL_FWD(a)),
					bidiag_control<decompose_mode>{mode}
				);
			}
		},
		#endif //!HBRS_MPL_ENABLE_MATLAB
		[](auto && a, auto mode) {
			auto sz_ = (*size)(a);
			auto m_ = (*m)(sz_);
			auto n_ = (*n)(sz_);
			
			if constexpr(
				(mode != decompose_mode::complete) || (hana::value(m_) < hana::value(n_))
			) {
				return detail::not_supported{};
			} else {
				return hana::make_tuple(
					detail::bidiag_impl_rtsam{},
					make_rtsam(HBRS_MPL_FWD(a)),
					bidiag_control<decompose_mode>{mode}
				);
			}
		},
		"SEQUENCE_TERMINATOR___REMOVED_BY_DROP_BACK"
	));
	
	hana::for_each(hana::cartesian_product(dimensions), [](auto const& cfg) {
		auto const& dataset_nr = hana::at_c<0>(cfg);
		auto const& mode = hana::at_c<1>(cfg);
		
		BOOST_TEST_MESSAGE("dataset_nr=" << dataset_nr);
		BOOST_TEST_MESSAGE(
			 "decompose_mode=" << 
			 (
				(mode == decompose_mode::complete) 
				? "complete" 
				: (
					(mode == decompose_mode::economy) 
					? "economy" 
					: "zero"
				)
			)
		);
		
		auto const& dataset = hana::at(datasets, dataset_nr);
		
		auto testcases = hana::transform(
			factories,
			[&](auto factory) { return factory(dataset, mode); }
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
				
				using hbrs::mpl::select;
				
				auto sz_ = (*size)(dataset);
				auto m_ = (*m)(sz_);
				auto n_ = (*n)(sz_);
				
				BOOST_TEST_MESSAGE("comparing bidiag_u of impl nr " << impl_idx_i << " and " << impl_idx_j);
				auto const& bidiag_u_i = (*at)(result_i, bidiag_u{});
				auto const& bidiag_u_j = (*at)(result_j, bidiag_u{});
				if ((m_ > n_) && (mode == decompose_mode::complete)) {
					auto rng = std::make_pair(make_matrix_index(0,0), make_matrix_size((int)m_, (int)n_));
					auto const bidiag_u_i_mxn = (*select)(bidiag_u_i, rng);
					auto const bidiag_u_j_mxn = (*select)(bidiag_u_j, rng);
					HBRS_MPL_TEST_MMEQ(bidiag_u_i_mxn, bidiag_u_j_mxn, true);
				} else {
					HBRS_MPL_TEST_MMEQ(bidiag_u_i, bidiag_u_j, true);
				}
				
				BOOST_TEST_MESSAGE("comparing bidiag_b of impl nr " << impl_idx_i << " and " << impl_idx_j);
				auto const& bidiag_b_i = (*at)(result_i, bidiag_b{});
				auto const& bidiag_b_j = (*at)(result_j, bidiag_b{});
				HBRS_MPL_TEST_MMEQ(bidiag_b_i, bidiag_b_j, false);
				
				BOOST_TEST_MESSAGE("comparing bidiag_v of impl nr " << impl_idx_i << " and " << impl_idx_j);
				auto const& bidiag_v_i = (*at)(result_i, bidiag_v{});
				auto const& bidiag_v_j = (*at)(result_j, bidiag_v{});
				if (
					(m_ < n_) && 
					((mode == decompose_mode::complete) || (mode == decompose_mode::zero))
				) {
					auto rng = std::make_pair(make_matrix_index(0,0), make_matrix_size((int)n_, (int)m_));
					auto const bidiag_v_i_mxn = (*select)(bidiag_v_i, rng);
					auto const bidiag_v_j_mxn = (*select)(bidiag_v_j, rng);
					HBRS_MPL_TEST_MMEQ(bidiag_v_i_mxn, bidiag_v_j_mxn, true);
				} else {
					HBRS_MPL_TEST_MMEQ(bidiag_v_i, bidiag_v_j, true);
				}
				
				BOOST_TEST_MESSAGE("comparing impl nr " << impl_idx_i << " and " << impl_idx_j << " done.");
				
				BOOST_TEST_PASSPOINT();
			};
			
			hana::for_each(hana::drop_back(results_indices), compare);
		}
		
		BOOST_TEST_PASSPOINT();
		
		hana::for_each(results_indices, [&](auto i) {
			auto impl_idx = hana::at(supported_indices, i);
			auto bidiag_result = hana::at(results, i);
			
			BOOST_TEST_MESSAGE("Testing properties of U, B and V computed by impl nr " << impl_idx);
			
			auto sz_ = (*size)(dataset);
			auto m_ = (*m)(sz_);
			auto n_ = (*n)(sz_);
			
			auto && u = (*at)(bidiag_result, bidiag_u{});
			auto && b = (*at)(bidiag_result, bidiag_b{});
			auto && v = (*at)(bidiag_result, bidiag_v{});
			
			auto u_sz = (*size)(u);
			auto u_m = (*m)(u_sz);
			auto u_n = (*n)(u_sz);
			auto b_sz = (*size)(b);
			auto b_m = (*m)(b_sz);
			auto b_n = (*n)(b_sz);
			auto v_sz = (*size)(v);
			auto v_m = (*m)(v_sz);
			auto v_n = (*n)(v_sz);
			
			if (
				(mode == decompose_mode::complete) ||
				((mode == decompose_mode::zero) && (m_ <= n_))
			) {
				BOOST_TEST( (*equal)(u_m,m_) );
				BOOST_TEST( (*equal)(u_n,m_) );
				
				BOOST_TEST( (*equal)(b_m,m_) );
				BOOST_TEST( (*equal)(b_n,n_) );
				
				BOOST_TEST( (*equal)(v_m,n_) );
				BOOST_TEST( (*equal)(v_n,n_) );
			} else if (
				(mode == decompose_mode::economy) ||
				((mode == decompose_mode::zero) && (m_ > n_))
			) {
				BOOST_TEST( (*equal)(u_m,m_) );
				if (m_ > n_) {
					BOOST_TEST( (*equal)(u_n,n_) );
				} else {
					BOOST_TEST( (*equal)(u_n,m_) );
				}
				
				if (m_ > n_) {
					BOOST_TEST( (*equal)(b_m,n_) );
					BOOST_TEST( (*equal)(b_n,n_) );
				} else if (m_ == n_) {
					BOOST_TEST( (*equal)(b_m,m_) );
					BOOST_TEST( (*equal)(b_n,n_) );
				} else if (m_ < n_) {
					BOOST_TEST( (*equal)(b_m,m_) );
					BOOST_TEST( (*equal)(b_n,m_) );
				}
				
				BOOST_TEST( (*equal)(v_m,n_) );
				if (m_ < n_) {
					BOOST_TEST( (*equal)(v_n,m_) );
				} else {
					BOOST_TEST( (*equal)(v_n,n_) );
				}
			} else {
				BOOST_ASSERT(false);
			}
			
			//TODO: Test if all matrix entries are zeros except for main and super diagonal
			
			BOOST_TEST_MESSAGE("Testing matrices for impl nr " << impl_idx << " done");
		});
		
		BOOST_TEST_PASSPOINT();
		
		hana::for_each(
			results_indices,
			[&](auto i) {
				auto impl_idx = hana::at(supported_indices, i);
				auto bidiag_result = hana::at(results, i);
				
				BOOST_TEST_MESSAGE("Rebuilding A'=U*B*V' for impl nr " << impl_idx);
				
				auto && u = (*at)(bidiag_result, bidiag_u{});
				auto && b = (*at)(bidiag_result, bidiag_b{});
				auto && v = (*at)(bidiag_result, bidiag_v{});
				
				auto rebuild = (*multiply)(u, multiply(b,transpose(v)));
				
				BOOST_TEST_MESSAGE("Comparing original data and reconstructed data computed by impl nr " << impl_idx);
				HBRS_MPL_TEST_MMEQ(dataset, rebuild, false);
			}
		);
		
		BOOST_TEST_MESSAGE("Comparing original and reconstructed datasets done.");
	});
}

BOOST_AUTO_TEST_SUITE_END()
