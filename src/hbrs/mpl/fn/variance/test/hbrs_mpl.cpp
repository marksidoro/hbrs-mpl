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

#define BOOST_TEST_MODULE fn_variance_hbrs_mpl_test
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


#include <hbrs/mpl/dt/srv.hpp>
#include <hbrs/mpl/fn/variance.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/transform.hpp>
#include <boost/hana/for_each.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/plus.hpp>
#include <boost/hana/length.hpp>
#include <boost/hana/minus.hpp>
#include <boost/hana/at.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/drop_back.hpp>
#include <boost/hana/value.hpp>
#include <boost/hana/less_equal.hpp>
#include <boost/hana/greater_equal.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/cartesian_product.hpp>
#include <array>

namespace utf = boost::unit_test;
namespace tt = boost::test_tools;

#define _TOL 0.000000001

BOOST_AUTO_TEST_SUITE(fn_variance_hbrs_mpl_test)

using hbrs::mpl::detail::environment_fixture;
BOOST_TEST_GLOBAL_FIXTURE(environment_fixture);

BOOST_AUTO_TEST_CASE(columns_variances, * utf::tolerance(_TOL)) {
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
	
	static constexpr auto column_variances_w0 = hana::make_tuple(
		make_srv(std::array<double, 3>{65.33333333333, 12266.33333333333, 1323.0}),
		make_srv(std::array<double, 4>{34.6025641025641, 242.1410256410256, 41.0256410256410, 280.1666666666667}),
		make_srv(std::array<double, 13>{636.916666666667, 472.916666666667, 488.25, 334.25, 492.333333333333, 452.916666666667, 1007.583333333333, 327., 474.666666666667, 313.666666666667, 295., 767., 843.666666666667})
	);
	
	static constexpr auto column_variances_w1 = hana::make_tuple(
		make_srv(std::array<double, 3>{43.555555555556, 8177.555555555555, 882.000000000000}),
		make_srv(std::array<double, 4>{31.9408284023669, 223.5147928994083, 37.8698224852071, 258.6153846153846}),
		make_srv(std::array<double, 13>{477.6875, 354.6875, 366.1875, 250.6875, 369.25, 339.6875, 755.6875, 245.25, 356., 235.25, 221.25, 575.25, 632.75})
	);
	
	static const auto dimensions = hana::make_tuple(
		hana::to_tuple(hana::make_range(hana::size_c<0>, hana::length(datasets))),
		hana::make_tuple(0., 1.)
	);
	
	hana::for_each(hana::cartesian_product(dimensions), [&](auto const& cfg) {
		auto const& dataset_nr = hana::at_c<0>(cfg);
		auto const& weight = hana::at_c<1>(cfg);
		
		BOOST_TEST_MESSAGE("dataset_nr=" << dataset_nr);
		BOOST_TEST_MESSAGE("weight=" << weight);
		auto const& dataset = hana::at(datasets, dataset_nr);
		
		auto funs = hana::drop_back(hana::make_tuple(
			#ifdef HBRS_MPL_ENABLE_MATLAB
			[](auto && dataset, auto && weight) {
				auto matrix = make_ml_matrix(HBRS_MPL_FWD(dataset));
				return (*hbrs::mpl::variance)(columns(matrix), HBRS_MPL_FWD(weight));
			},
			#endif
			#ifdef HBRS_MPL_ENABLE_ELEMENTAL
			[](auto && dataset, auto && weight) {
				auto matrix = make_el_matrix(HBRS_MPL_FWD(dataset));
				return (*hbrs::mpl::variance)(columns(matrix), HBRS_MPL_FWD(weight));
			},
			[](auto && dataset, auto && weight) {
				static El::Grid grid{}; // grid is static because reference to grid is required by El::DistMatrix<...>
				auto matrix = make_el_dist_matrix(grid, make_el_matrix(HBRS_MPL_FWD(dataset)));
				return (*hbrs::mpl::variance)(columns(matrix), HBRS_MPL_FWD(weight));
			},
			#endif
			"SEQUENCE_TERMINATOR___REMOVED_BY_DROP_BACK"
		));
		
		auto results = hana::transform(funs, [&](auto f) { return f(dataset, weight); });
		
		hana::for_each(
			hana::make_range(hana::ushort_c<0>, hana::length(results)),
			[&](auto i) {
				BOOST_TEST_MESSAGE("Comparing variances computed by impl nr " << i);
				
				auto const& result = hana::at(results, i);
				
				BOOST_ASSERT((weight==0) || (weight==1));
				
				if (weight == 0) {
					HBRS_MPL_TEST_VVEQ(result, hana::at(column_variances_w0, dataset_nr), false);
				} else {
					HBRS_MPL_TEST_VVEQ(result, hana::at(column_variances_w1, dataset_nr), false);
				}
				
			}
		);
		BOOST_TEST_MESSAGE("Comparing variances completed");
	});
}

BOOST_AUTO_TEST_CASE(rows_variances, * utf::tolerance(_TOL)) {
	//TODO: Implement!
}

BOOST_AUTO_TEST_SUITE_END()
