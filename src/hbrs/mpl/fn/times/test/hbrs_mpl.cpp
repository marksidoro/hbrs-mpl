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

#define BOOST_TEST_MODULE fn_times_hbrs_mpl_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/config.hpp>

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
	#include <hbrs/mpl/dt/el_dist_matrix.hpp>
	#include <hbrs/mpl/dt/el_dist_vector.hpp>
#endif // !HBRS_MPL_ENABLE_ELEMENTAL

#include <hbrs/mpl/detail/test.hpp>
#include <hbrs/mpl/detail/gather.hpp>
#include <hbrs/mpl/detail/not_supported.hpp>

#include <hbrs/mpl/dt/ctsav.hpp>
#include <hbrs/mpl/dt/scv.hpp>
#include <hbrs/mpl/dt/sm.hpp>
#include <hbrs/mpl/dt/storage_order.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>

#include <hbrs/mpl/fn/times.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/multiply.hpp>
#include <hbrs/mpl/fn/expand.hpp>

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

#define _TOL 0.000000000001

BOOST_AUTO_TEST_SUITE(fn_times_hbrs_mpl_test)

using hbrs::mpl::detail::environment_fixture;
BOOST_TEST_GLOBAL_FIXTURE(environment_fixture);

// TODO: Add unit tests for non-distributed times() impls

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
BOOST_AUTO_TEST_CASE(times_el_dist_matrix_expr, * utf::tolerance(_TOL)) {
	using namespace hbrs::mpl;
	
	static constexpr auto datasets = hana::make_tuple(
		hana::make_tuple(
			make_sm(
				make_ctsav(detail::mat_k), make_matrix_size(hana::size_c<detail::mat_k_m>, hana::size_c<detail::mat_k_n>), row_major_c
			),
			make_scv(std::array<double, 24>{
				1,
				2,
				3,
				4,
				5,
				6,
				7,
				8,
				9,
				10,
				11,
				12,
				13,
				14,
				15,
				16,
				17,
				18,
				19,
				20,
				21,
				22,
				23,
				24
			}),
			make_ctsam(
				std::array<double, 24*24>{
					47,  -14,    0,  -47,   45,   14,  -26,  -18,   -8,   10,   27,  -45,   10,  -16,    8,    5,   -1,  -15,  -27,  -24,  -41,    2,  -40,   39,
					76,  -76,  -56,  -44,  -70,   -2,  -58,   -4,   30,   24,   76,   58,   84,   80,  -88,  -40,  -12,   88,  -66,  -18,  -48,  -54,  -48,  -34,
					123,   -6,  129,  -45,   39,  -69,  -99, -117,   72,  -33,  141,  144,   78,  -39,  -81,   72,  -15,  114,  -81,   27,   90,   -3,  -48,   60,
					-4,  -36,   -4, -104,  -56,  -64,   64,   88, -132,  -44, -128,   64,  152, -156,  -60, -124,  -76,   20,  -24,  -96, -188,   48,   72, -120,
					245, -220,  -60, -125, -210,  -40,  -15,   45,  125,  220,  -95,   25,  160,  140,  160,   95,    5,   60,  -95,   50,  215,   90, -180, -235,
					78,  264, -198,   84,  216, -168,   96,  168, -222,  -84, -120,   96,  156,  -66, -288, -192,    6,   54,  252,  126,  138,  -60,  132,  144,
					91, -175,  294,  322, -112,   49,    0,  -49, -336,  126,  210,   49,  -56, -182, -322,  -91,  224, -203,  -49, -196,   -7,  -91, -273,    0,
					248, -296, -296,   56,  312,  -56, -280, -272,  192,    8,   32, -264,  360,  -80, -264,  104,  232, -160, -256, -304,   64,  392,  120,  -16,
					234,  288,  108,  -54, -252,   18,  -81,  351,  297,   90,  261, -432,  360, -360,  135,  252,  126,  -27,  360, -180, -234, -414,   -9,  360,
					300, -140,  470, -210, -440, -320, -490, -380,   90,  250, -270, -240,  440, -370,  230, -420, -120, -270,  480, -180,  -40,  390,  280,  110,
					429, -517, -286, -484, -297,  110, -440, -418,  -66,  110,  264,  528,  -11,  484,  165,  473,  341,  374,  -66,  -88,  506,  451,  242,  132,
					240, -156,  168, -444, -564, -528,  588,  480, -432,  108,  300,  312,  -12,  552,  -60,  336,   36, -372, -468,   12,   60,  360,  480,  432,
					403,  598,  234,  247,  273,  325,  403,  546,   91,  247, -507, -416,  403, -572,  611, -572, -169, -104, -611, -403,  416,  -91, -559,  364,
					574,  -14,  364, -252,  350, -336, -364, -294,  -42,  350,  644, -336, -112,  252,  210, -140, -420,  672,   84,  -98,  672, -266, -252, -112,
					555,  450,  360,  675, -330,   15,  645,  390, -735,  -75, -750, -525,  480, -690,  450,   45,  -15, -300,  570,  -30,  345, -510,   45, -615,
					656, -576, -176, -752,  288,  320, -240,  400, -256, -672,  432, -576,  416, -688,  -80, -128, -256,  320,  272, -608, -256, -512,  240, -368,
					221, -136,  272, -102,  272,  663, -510, -204, -578, -459,  544,  629,  663,   34, -119,  272,  765,  289, -527,  153,  136, -136, -153, -595,
					720,  756, -594, -216, -612,  828, -450,  126,  522,  738,  666,  144,  846, -720,  594,  234,  756,   72, -234, -486, -702, -738,  576, -396,
					418,  551,  399,  513, -722,   95,  228, -798, -361, -665, -798,   95,  -19,  608, -798, -399, -855,  380,  -76, -228,  779,  190,  418, -114,
					-100,  920, -940,  600,    0, -720,  -60, -900,   60,  660, -200, -720,  660,  640, -740, -140,  480,  340,  960,  160,  760,  -60,  940,   60,
					966,  336, -462, -651,  966, -735, -315,   63, -693,   84, -504,  735, 1008,  462, -693,-1008, -483, -672, -714, -525,  672,  420,   63,  -84,
					1012,-1012, -990,  -22, -352, -528,  726,  616,  220, 1100,  660,  264,  462, -770, -242, 1056, -176, -814,  792, -462, -528,  440, -374,  836,
					782,  805, -920, -115,  207,  782,  207,  989, -552, -966, -161, -345, -207,  368,  759, -759,  115, 1150,  322,  276,  207,  322, -897,   46,
					1128, 1032,  768,  360, -672, -600,  120, -888,  360, -144,  984,   24,  -72,   48,  720, -936, 1056, -792, -288, -552,-1152,-1128,  264, 1056
				},
				make_matrix_size(hana::size_c<24>, hana::size_c<24>),
				row_major_c
			)
		),
		hana::make_tuple(
			make_sm(
				make_ctsav(detail::mat_k), make_matrix_size(hana::size_c<detail::mat_k_m>, hana::size_c<detail::mat_k_n>), row_major_c
			),
			make_srv(std::array<double, 24>{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24}),
			make_ctsam(
				std::array<double, 24*24>{
					47, -28,   0,-188, 225,  84,-182,-144, -72, 100, 297,-540, 130,-224, 120,  80, -17,-270,-513, -480,-861,  44,-920, 936 ,
					38, -76, -84, -88,-175,  -6,-203, -16, 135, 120, 418, 348, 546, 560,-660,-320,-102, 792,-627, -180,-504,-594,-552,-408 ,
					41,  -4, 129, -60,  65,-138,-231,-312, 216,-110, 517, 576, 338,-182,-405, 384, -85, 684,-513,  180, 630, -22,-368, 480 ,
					-1, -18,  -3,-104, -70, -96, 112, 176,-297,-110,-352, 192, 494,-546,-225,-496,-323,  90,-114, -480,-987, 264, 414,-720 ,
					49, -88, -36,-100,-210, -48, -21,  72, 225, 440,-209,  60, 416, 392, 480, 304,  17, 216,-361,  200, 903, 396,-828,-1128,
					13,  88, -99,  56, 180,-168, 112, 224,-333,-140,-220, 192, 338,-154,-720,-512,  17, 162, 798,  420, 483,-220, 506, 576 ,
					13, -50, 126, 184, -80,  42,   0, -56,-432, 180, 330,  84,-104,-364,-690,-208, 544,-522,-133, -560, -21,-286,-897,   0 ,
					31, -74,-111,  28, 195, -42,-245,-272, 216,  10,  44,-396, 585,-140,-495, 208, 493,-360,-608, -760, 168,1078, 345, -48 ,
					26,  64,  36, -24,-140,  12, -63, 312, 297, 100, 319,-576, 520,-560, 225, 448, 238, -54, 760, -400,-546,-1012, -23, 960,
					30, -28, 141, -84,-220,-192,-343,-304,  81, 250,-297,-288, 572,-518, 345,-672,-204,-486, 912, -360, -84, 858, 644, 264 ,
					39, -94, -78,-176,-135,  60,-280,-304, -54, 100, 264, 576, -13, 616, 225, 688, 527, 612,-114, -160, 966, 902, 506, 288 ,
					20, -26,  42,-148,-235,-264, 343, 320,-324,  90, 275, 312, -13, 644, -75, 448,  51,-558,-741,   20, 105, 660, 920, 864 ,
					31,  92,  54,  76, 105, 150, 217, 336,  63, 190,-429,-384, 403,-616, 705,-704,-221,-144,-893, -620, 672,-154,-989, 672 ,
					41,  -2,  78, -72, 125,-144,-182,-168, -27, 250, 506,-288,-104, 252, 225,-160,-510, 864, 114, -140,1008,-418,-414,-192 ,
					37,  60,  72, 180,-110,   6, 301, 208,-441, -50,-550,-420, 416,-644, 450,  48, -17,-360, 722,  -40, 483,-748,  69,-984 ,
					41, -72, -33,-188,  90, 120,-105, 200,-144,-420, 297,-432, 338,-602, -75,-128,-272, 360, 323, -760,-336,-704, 345,-552 ,
					13, -16,  48, -24,  80, 234,-210, -96,-306,-270, 352, 444, 507,  28,-105, 256, 765, 306,-589,  180, 168,-176,-207,-840 ,
					40,  84, -99, -48,-170, 276,-175,  56, 261, 410, 407,  96, 611,-560, 495, 208, 714,  72,-247, -540,-819,-902, 736,-528 ,
					22,  58,  63, 108,-190,  30,  84,-336,-171,-350,-462,  60, -13, 448,-630,-336,-765, 360, -76, -240, 861, 220, 506,-144 ,
					-5,  92,-141, 120,   0,-216, -21,-360,  27, 330,-110,-432, 429, 448,-555,-112, 408, 306, 912,  160, 798, -66,1081,  72 ,
					46,  32, -66,-124, 230,-210,-105,  24,-297,  40,-264, 420, 624, 308,-495,-768,-391,-576,-646, -500, 672, 440,  69, -96 ,
					46, -92,-135,  -4, -80,-144, 231, 224,  90, 500, 330, 144, 273,-490,-165, 768,-136,-666, 684, -420,-504, 440,-391, 912 ,
					34,  70,-120, -20,  45, 204,  63, 344,-216,-420, -77,-180,-117, 224, 495,-528,  85, 900, 266,  240, 189, 308,-897,  48 ,
					47,  86,  96,  60,-140,-150,  35,-296, 135, -60, 451,  12, -39,  28, 450,-624, 748,-594,-228, -460,-1008,-1034, 253,1056
				},
				make_matrix_size(hana::size_c<24>, hana::size_c<24>),
				row_major_c
			)
		)
	);
	
	static constexpr auto dimensions = hana::make_tuple(
		hana::to_tuple(hana::make_range(hana::size_c<0>, hana::length(datasets)))
	);
	
	static constexpr auto is_column_vector = [](auto const& v) {
		using v_tag = hana::tag_of_t<decltype(v)>;
		static constexpr bool is_column_vector = 
			std::is_same_v<v_tag, scv_tag> || std::is_same_v<v_tag, el_dist_column_vector_tag>;
		static constexpr bool is_row_vector = 
			std::is_same_v<v_tag, srv_tag> || std::is_same_v<v_tag, el_dist_row_vector_tag>;
		static_assert(is_column_vector || is_row_vector, "unsupported vector type");
		static_assert(!(is_column_vector && is_row_vector), "internal error");
		return hana::bool_c<is_column_vector>;
	};
	
	static constexpr auto make_el_dist_vector = [&](auto v) {
		static El::Grid grid{El::mpi::COMM_WORLD}; // grid is static because reference to grid is required by El::DistMatrix<...>
		if constexpr(decltype(is_column_vector(v))::value) {
			return make_el_dist_column_vector(grid, make_el_column_vector(std::move(v)));
		} else {
			return make_el_dist_row_vector(grid, make_el_row_vector(std::move(v)));
		}
	};
	
	static constexpr auto factories = hana::drop_back(hana::make_tuple(
		[&](auto && dataset) {
			static El::Grid grid{El::mpi::COMM_WORLD}; // grid is static because reference to grid is required by El::DistMatrix<...>
			auto A = hana::at(dataset, hana::size_c<0>);
			auto v = hana::at(dataset, hana::size_c<1>);
			auto AV_cmp = hana::at(dataset, hana::size_c<2>);
			
			auto A_star_star = make_el_dist_matrix(grid, make_el_matrix(std::move(A)));
			auto v_star_star = make_el_dist_vector(std::move(v));
			
			return hana::make_tuple(
				detail::times_impl_el_dist_matrix_expand_expr_el_dist_matrix{},
				A_star_star,
				make_expression(
					expand,
					hana::make_tuple(
						v_star_star,
						(*size)(A_star_star)
					)
				),
				v_star_star,
				AV_cmp
			);
		},
		[&](auto && dataset) {
			static El::Grid grid{El::mpi::COMM_WORLD}; // grid is static because reference to grid is required by El::DistMatrix<...>
			auto A = hana::at(dataset, hana::size_c<0>);
			auto v = hana::at(dataset, hana::size_c<1>);
			auto AV_cmp = hana::at(dataset, hana::size_c<2>);
			
			auto A_star_star = make_el_dist_matrix(grid, make_el_matrix(std::move(A)));
			auto v_star_star = make_el_dist_vector(std::move(v));
			
			auto A_mc_mr = make_el_dist_matrix(
				A_star_star,
				make_matrix_distribution(
					hana::integral_constant<El::Dist, El::MC>{},
					hana::integral_constant<El::Dist, El::MR>{},
					hana::integral_constant<El::DistWrap, El::ELEMENT>{}
				)
			);
			
			return hana::make_tuple(
				detail::times_impl_el_dist_matrix_expand_expr_el_dist_matrix{},
				A_mc_mr,
				make_expression(
					expand,
					hana::make_tuple(
						v_star_star,
						(*size)(A_mc_mr)
					)
				),
				v_star_star,
				AV_cmp
			);
		},
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
					hana::drop_front(hana::drop_back(hana::drop_back(testcase))),
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
				
				HBRS_MPL_TEST_MMEQ(result_i, result_j, false);
				
				BOOST_TEST_MESSAGE("comparing impl nr " << impl_idx_i << " and " << impl_idx_j << " done.");
				
				BOOST_TEST_PASSPOINT();
			};
			
			hana::for_each(hana::drop_back(results_indices), compare);
		}
		
		BOOST_TEST_PASSPOINT();
		
		hana::for_each(
			results_indices,
			[&](auto i) {
				auto impl_idx = hana::at(supported_indices, i);
				BOOST_TEST_MESSAGE("Testing A.*V computed by impl nr " << impl_idx);
				
				auto testcase = hana::at(testcases, impl_idx);
				auto A = hana::at(testcase, hana::size_c<1>);
				auto v = hana::at(testcase, hana::size_c<3>);
				auto AV_ref = hana::at(testcase, hana::size_c<4>);
				auto AV = hana::at(results, i);
				
				HBRS_MPL_TEST_MMEQ(AV_ref, AV, false);
				
				auto A_star_star = make_el_dist_matrix(
					A,
					make_matrix_distribution(
						hana::integral_constant<El::Dist, El::STAR>{},
						hana::integral_constant<El::Dist, El::STAR>{},
						hana::integral_constant<El::DistWrap, El::ELEMENT>{}
					)
				);
				auto AV_cmp = A_star_star;
				auto A_sz = (*size)(A);
				auto A_m = (*m)(A_sz);
				auto A_n = (*n)(A_sz);
				
				decltype(auto) AV_cmp_lcl = AV_cmp.data().Matrix();
				auto v_lcl = v.data().Matrix();
				
				if constexpr(decltype(is_column_vector(v))::value) {
					for(El::Int j = 0; j < A_n; ++j) {
						for(El::Int i = 0; i < A_m; ++i) {
							El::Int il = AV_cmp.data().LocalCol(i);
							El::Int jl = AV_cmp.data().LocalRow(j);
							AV_cmp_lcl.Set(
								il, jl,
								AV_cmp_lcl.Get(il, jl) * v_lcl.Get(v.data().LocalCol(i), v.data().LocalRow(0))
							);
						}
					}
				} else {
					for(El::Int j = 0; j < A_n; ++j) {
						for(El::Int i = 0; i < A_m; ++i) {
							El::Int il = AV_cmp.data().LocalCol(i);
							El::Int jl = AV_cmp.data().LocalRow(j);
							AV_cmp_lcl.Set(
								il, jl,
								AV_cmp_lcl.Get(il, jl) * v_lcl.Get(v.data().LocalCol(0), v.data().LocalRow(j))
							);
						}
					}
				}
				
				HBRS_MPL_TEST_MMEQ(AV_cmp, AV, false);
			}
		);
		BOOST_TEST_MESSAGE("Testing A.*V done.");
	});
}
#endif // !HBRS_MPL_ENABLE_ELEMENTAL

BOOST_AUTO_TEST_SUITE_END()
