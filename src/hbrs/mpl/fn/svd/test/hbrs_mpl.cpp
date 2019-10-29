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

#define BOOST_TEST_MODULE fn_svd_hbrs_mpl_test
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
#include <hbrs/mpl/dt/range.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>
#include <hbrs/mpl/dt/decompose_mode.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/svd.hpp>
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

BOOST_AUTO_TEST_SUITE(fn_svd_hbrs_mpl_test)

using hbrs::mpl::detail::environment_fixture;
BOOST_TEST_GLOBAL_FIXTURE(environment_fixture);

BOOST_AUTO_TEST_CASE(svd_comparison, * utf::tolerance(_TOL)) {
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
		)
	);
	
	static constexpr auto dimensions = hana::make_tuple(
		hana::to_tuple(hana::make_range(hana::size_c<0>, hana::length(datasets))),
		hana::make_tuple(complete_decompose_c, economy_decompose_c, zero_decompose_c)
	);
	
	static constexpr auto factories = hana::drop_back(hana::make_tuple(
		#ifdef HBRS_MPL_ENABLE_MATLAB
		[](auto && a, auto mode) {
			return hana::make_tuple(
				detail::svd_impl_level0_ml_matrix{},
				make_ml_matrix(HBRS_MPL_FWD(a)),
				svd_control<decompose_mode>{mode}
			);
		},
		[](auto && a, auto mode) {
			//TODO: Remove not_supported once svd impl has been fixed.
			if constexpr(true) {
				return detail::not_supported{};
			} else {
				return hana::make_tuple(
					detail::svd_impl_level1_ml_matrix{},
					make_ml_matrix(HBRS_MPL_FWD(a)),
					svd_control<decompose_mode>{mode}
				);
			}
		},
		#endif
		#ifdef HBRS_MPL_ENABLE_ELEMENTAL
		[](auto && a, auto mode) {
			return hana::make_tuple(
				detail::svd_impl_el_matrix{},
				make_el_matrix(HBRS_MPL_FWD(a)),
				svd_control<decompose_mode>{mode}
			);
		},
		[](auto && a, auto mode) {
			// matrix has [STAR,STAR] distribution
			auto sz_ = (*size)(a);
			auto m_ = (*m)(sz_);
			auto n_ = (*n)(sz_);
			
			if constexpr(
				(mode == decompose_mode::complete) ||
				(mode == decompose_mode::zero && hana::value(m_) <= hana::value(n_))
			) {
				return detail::not_supported{};
			} else {
				static El::Grid grid{El::mpi::COMM_WORLD}; // grid is static because reference to grid is required by El::DistMatrix<...>
				return hana::make_tuple(
					detail::svd_impl_el_dist_matrix{},
					make_el_dist_matrix(
						grid,
						make_el_matrix(HBRS_MPL_FWD(a))
					),
					svd_control<decompose_mode>{mode}
				);
			}
		},
		[](auto && a, auto mode) {
			// matrix has [MC,MR] distribution
			typedef decltype(a.at(matrix_index<std::size_t, std::size_t>{0u,0u})) Ring;
			typedef std::decay_t<Ring> _Ring_;
			
			auto sz_ = (*size)(a);
			auto m_ = (*m)(sz_);
			auto n_ = (*n)(sz_);
			
			if constexpr(
				(mode == decompose_mode::complete) ||
				(mode == decompose_mode::zero && hana::value(m_) <= hana::value(n_))
			) {
				return detail::not_supported{};
			} else {
				static El::Grid grid{El::mpi::COMM_WORLD}; // grid is static because reference to grid is required by El::DistMatrix<...>
				return hana::make_tuple(
					detail::svd_impl_el_dist_matrix{},
					el_dist_matrix<_Ring_, El::MC, El::MR>{
						make_el_dist_matrix(
							grid,
							make_el_matrix(HBRS_MPL_FWD(a))
						).data()
					},
					svd_control<decompose_mode>{mode}
				);
			}
		},
		#endif
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
				//TODO: Take ++j if j not supported!
				
				using hbrs::mpl::select;
				
				auto sz_ = (*size)(dataset);
				auto m_ = (*m)(sz_);
				auto n_ = (*n)(sz_);
				
				BOOST_TEST_MESSAGE("comparing svd_u of impl nr " << impl_idx_i << " and " << impl_idx_j);
				auto const& svd_u_i = (*at)(result_i, svd_u{});
				auto const& svd_u_j = (*at)(result_j, svd_u{});
				if ((m_ > n_) && (mode == decompose_mode::complete)) {
					auto rng = std::make_pair(make_matrix_index(0,0), make_matrix_size((int)m_, (int)n_));
					auto const svd_u_i_mxn = (*select)(svd_u_i, rng);
					auto const svd_u_j_mxn = (*select)(svd_u_j, rng);
					HBRS_MPL_TEST_MMEQ(svd_u_i_mxn, svd_u_j_mxn, true);
				} else {
					HBRS_MPL_TEST_MMEQ(svd_u_i, svd_u_j, true);
				}
				
				BOOST_TEST_MESSAGE("comparing svd_s of impl nr " << impl_idx_i << " and " << impl_idx_j);
				auto const& svd_s_i = (*at)(result_i, svd_s{});
				auto const& svd_s_j = (*at)(result_j, svd_s{});
				HBRS_MPL_TEST_MMEQ(svd_s_i, svd_s_j, false);
				
				BOOST_TEST_MESSAGE("comparing svd_v of impl nr " << impl_idx_i << " and " << impl_idx_j);
				auto const& svd_v_i = (*at)(result_i, svd_v{});
				auto const& svd_v_j = (*at)(result_j, svd_v{});
				if (
					(m_ < n_) && 
					((mode == decompose_mode::complete) || (mode == decompose_mode::zero))
				) {
					auto rng = std::make_pair(make_matrix_index(0,0), make_matrix_size((int)n_, (int)m_));
					auto const svd_v_i_mxn = (*select)(svd_v_i, rng);
					auto const svd_v_j_mxn = (*select)(svd_v_j, rng);
					HBRS_MPL_TEST_MMEQ(svd_v_i_mxn, svd_v_j_mxn, true);
				} else {
					HBRS_MPL_TEST_MMEQ(svd_v_i, svd_v_j, true);
				}
				
				BOOST_TEST_MESSAGE("comparing impl nr " << impl_idx_i << " and " << impl_idx_j << " done.");
				
				BOOST_TEST_PASSPOINT();
			};
			
			hana::for_each(hana::drop_back(results_indices), compare);
		}
		
		BOOST_TEST_PASSPOINT();
		
		hana::for_each(results_indices, [&](auto i) {
			auto impl_idx = hana::at(supported_indices, i);
			auto svd_result = hana::at(results, i);
			
			BOOST_TEST_MESSAGE("Testing properties of U, S and V computed by impl nr " << impl_idx);
			
			auto sz_ = (*size)(dataset);
			auto m_ = (*m)(sz_);
			auto n_ = (*n)(sz_);
			
			auto && u = (*at)(svd_result, svd_u{});
			auto && s = (*at)(svd_result, svd_s{});
			auto && v = (*at)(svd_result, svd_v{});
			
			auto u_sz = (*size)(u);
			auto u_m = (*m)(u_sz);
			auto u_n = (*n)(u_sz);
			auto s_sz = (*size)(s);
			auto s_m = (*m)(s_sz);
			auto s_n = (*n)(s_sz);
			auto v_sz = (*size)(v);
			auto v_m = (*m)(v_sz);
			auto v_n = (*n)(v_sz);
			
			if (
				(mode == decompose_mode::complete) ||
				((mode == decompose_mode::zero) && (m_ <= n_))
			) {
				BOOST_TEST( (*equal)(u_m,m_) );
				BOOST_TEST( (*equal)(u_n,m_) );
				
				BOOST_TEST( (*equal)(s_m,m_) );
				BOOST_TEST( (*equal)(s_n,n_) );
				
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
					BOOST_TEST( (*equal)(s_m,n_) );
					BOOST_TEST( (*equal)(s_n,n_) );
				} else if (m_ == n_) {
					BOOST_TEST( (*equal)(s_m,m_) );
					BOOST_TEST( (*equal)(s_n,n_) );
				} else if (m_ < n_) {
					BOOST_TEST( (*equal)(s_m,m_) );
					BOOST_TEST( (*equal)(s_n,m_) );
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
			
			if (
				(m_ > n_) && 
				((mode==decompose_mode::economy) || (mode==decompose_mode::zero))
			) {
				BOOST_TEST_MESSAGE("Skipping U*U'==I for impl nr " << impl_idx);
			} else {
				BOOST_TEST_MESSAGE("Testing U*U'==I for impl nr " << impl_idx);
				auto uxut = (*multiply)(u, transpose(u));
				BOOST_TEST((*size)(uxut) == make_matrix_size(m_, m_));
				HBRS_MPL_TEST_IS_IDENTITY(uxut);
			}
			
			if ( (m_ < n_) && (mode==decompose_mode::economy) ) {
				BOOST_TEST_MESSAGE("Skipping V*V'==I for impl nr " << impl_idx);
			} else {
				BOOST_TEST_MESSAGE("Testing V*V'==I for impl nr " << impl_idx);
				auto vxvt = (*multiply)(v, transpose(v));
				BOOST_TEST((*size)(vxvt) == make_matrix_size(n_, n_));
				HBRS_MPL_TEST_IS_IDENTITY(vxvt);
			}
			
			BOOST_TEST_MESSAGE("Testing unitary matrices for impl nr " << impl_idx << " done");
		});
		
		BOOST_TEST_PASSPOINT();
		
		hana::for_each(
			results_indices,
			[&](auto i) {
				auto impl_idx = hana::at(supported_indices, i);
				auto svd_result = hana::at(results, i);
				
				BOOST_TEST_MESSAGE("Rebuilding B=U*S*V' for impl nr " << impl_idx);
				
				auto && u = (*at)(svd_result, svd_u{});
				auto && s = (*at)(svd_result, svd_s{});
				auto && v = (*at)(svd_result, svd_v{});
				
				auto rebuild = (*multiply)(u, multiply(s,transpose(v)));
				
				BOOST_TEST_MESSAGE("Comparing original data and reconstructed data computed by impl nr " << impl_idx);
				HBRS_MPL_TEST_MMEQ(dataset, rebuild, false);
			}
		);
		
		BOOST_TEST_MESSAGE("Comparing original and reconstructed datasets done.");
	});
	
/*

complete list of test failures:

3x3 matrix A
  decompose_mode=complete / decompose_mode=economy / decompose_mode=zero
    matlab0 <u,s,v> == matlab1 <u,s,v>
    matlab1 <u,s,v> == el_mat0 <u,s,v>
    matlab0: U*U'==I 
             V*V'==I
    matlab1: U*U'==I 
             V*V'==I
    el_mat0: U*U'==I 
             V*V'==I
    matlab0: U*S*V'== A
    matlab1: U*S*V'== A
    el_mat0: U*S*V'== A


13x4 matrix A
  decompose_mode=complete
    matlab0 <u,s,v> == matlab1 <u,s,v>
    matlab1 <u> != el_mat0 <u> && matlab1 <s,v> == el_mat0 <s,v>
    matlab0: U*U'==I
             V*V'==I
    matlab1: U*U'==I
             V*V'==I
    el_mat0: U*U'==I
             V*V'==I
    matlab0: U*S*V'== A
    matlab1: U*S*V'== A
    el_mat0: U*S*V'== A

  decompose_mode=economy
    matlab0 <u,s,v> == matlab1 <u,s,v>
    matlab1 <u,s,v> == el_mat0 <u,s,v>
    matlab0: U*U'!=I
             V*V'==I
    matlab1: U*U'!=I
             V*V'==I
    el_mat0: U*U'!=I
             V*V'==I
    matlab0: U*S*V'== A
    matlab1: U*S*V'== A
    el_mat0: U*S*V'== A

  decompose_mode=zero
    matlab0 <u,s,v> == matlab1 <u,s,v>
    matlab1 <u,s,v> == el_mat0 <u,s,v>
    matlab0: U*U'!=I
             V*V'==I
    matlab1: U*U'!=I
             V*V'==I
    el_mat0: U*U'!=I
             V*V'==I
    matlab0: U*S*V'== A
    matlab1: U*S*V'== A
    el_mat0: U*S*V'== A


4x13 matrix A
  decompose_mode=complete
    matlab0 <u,s,v> == matlab1 <u,s,v>
    matlab1 <u,s> == el_mat0 <u,s> && matlab1 <v> != el_mat0 <v>
    matlab0: U*U'==I 
             V*V'==I
    matlab1: U*U'==I 
             V*V'==I
    el_mat0: U*U'==I 
             V*V'==I
    matlab0: U*S*V'== A
    matlab1: U*S*V'== A
    el_mat0: U*S*V'== A

  decompose_mode=economy
    matlab0 <u,s,v> == matlab1 <u,s,v>
    matlab1 <u,s,v> == el_mat0 <u,s,v>
    matlab0: U*U'==I 
             V*V'!=I
    matlab1: U*U'==I 
             V*V'!=I
    el_mat0: U*U'==I 
             V*V'!=I
    matlab0: U*S*V'== A
    matlab1: U*S*V'== A
    el_mat0: U*S*V'== A

  decompose_mode=zero
    matlab0 <u,s,v> == matlab1 <u,s,v>
    matlab1 <u,s> == el_mat0 <u,s> && matlab1 <v> != el_mat0 <v>
    matlab0: U*U'==I 
             V*V'==I
    matlab1: U*U'==I 
             V*V'==I
    el_mat0: U*U'==I 
             V*V'==I
    matlab0: U*S*V'== A
    matlab1: U*S*V'== A
    el_mat0: U*S*V'== A


summary of expected test failures:
13x4 matrix A : decompose_mode=complete : matlab1 <u> != el_mat0 <u> && matlab1 <s,v> == el_mat0 <s,v>
13x4 matrix A : decompose_mode=economy : matlab0: U*U'!=I
13x4 matrix A : decompose_mode=economy : matlab1: U*U'!=I
13x4 matrix A : decompose_mode=economy : el_mat0: U*U'!=I
13x4 matrix A : decompose_mode=zero : matlab0: U*U'!=I
13x4 matrix A : decompose_mode=zero : matlab1: U*U'!=I
13x4 matrix A : decompose_mode=zero : el_mat0: U*U'!=I
4x13 matrix A : decompose_mode=complete : matlab1 <u,s> == el_mat0 <u,s> && matlab1 <v> != el_mat0 <v>
4x13 matrix A : decompose_mode=economy : matlab0: V*V'!=I
4x13 matrix A : decompose_mode=economy : matlab1: V*V'!=I
4x13 matrix A : decompose_mode=economy : el_mat0: V*V'!=I
4x13 matrix A : decompose_mode=zero : matlab1 <u,s> == el_mat0 <u,s> && matlab1 <v> != el_mat0 <v>

reasons for test failures: 
 - U*U'!=I and V*V'!=I because in decompose_mode=economy n columns of U / m columns of V are returned thus U/V are not 
   necessarly orthogonal anymore.
 - matlab1 <u> != el_mat0 <u> && matlab1 <s,v> == el_mat0 <s,v> 
   and
   matlab1 <u,s> == el_mat0 <u,s> && matlab1 <v> != el_mat0 <v>
   because in decompose_mode=complete or decompose_mode=zero with m<n only the first min(m,n) columns of u/v must be equal.
   There may exist multiple orthogonal matrices where the first min(m,n) columns are equal and the last 
   (max(m,n)-min(m,n)) columns might be different.


 */
}

BOOST_AUTO_TEST_CASE(svd_rtsam, * utf::tolerance(0.000000001)) {
	using namespace hbrs::mpl;
	using hbrs::mpl::decompose_mode;
	
	rtsam<double, storage_order::row_major> A{
		{1, 2, 3,
		 4, 5, 6,
		 7, 8, 9},
		make_matrix_size(3,3)
	};

	rtsam<double, storage_order::row_major> C{
		{1, 2,  3,
		 4, 5,  6,
		 7, 8,  9,
		10, 11, 12},
		make_matrix_size(4,3)
	};

	rtsam<double, storage_order::row_major> D{
		{1, 0,  0,  0,
		 0, 6,  7,  0,
		 0, 0, 11, 12,
		 0, 0,  0,  0},
		 make_matrix_size(4,4)
	};
	
	auto ASVD = (*svd)(A, make_svd_control(decompose_mode::complete));
	auto CSVD = (*svd)(C, make_svd_control(decompose_mode::complete));
	auto DSVD = (*svd)(D, make_svd_control(decompose_mode::complete));

	auto rA = (*multiply)(multiply(ASVD.u(), ASVD.s()), transpose(ASVD.v()));
	auto rC = (*multiply)(multiply(CSVD.u(), CSVD.s()), transpose(CSVD.v()));
	auto rD = (*multiply)(multiply(DSVD.u(), DSVD.s()), transpose(DSVD.v()));
	
	HBRS_MPL_TEST_MMEQ(A, rA, false);
	HBRS_MPL_TEST_MMEQ(C, rC, false);
	HBRS_MPL_TEST_MMEQ(D, rD, false);
}

BOOST_AUTO_TEST_SUITE_END()

