/* Copyright (c) 2016-2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_DETAIL_TEST_IMPL_HPP
#define HBRS_MPL_DETAIL_TEST_IMPL_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/detail/environment.hpp>
#include <hbrs/mpl/detail/gather.hpp>
#include <hbrs/mpl/detail/mpi.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/equal.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <hbrs/mpl/detail/integral_value.hpp>
#include <boost/test/results_collector.hpp>
#include <cstdlib>

#if BOOST_VERSION <= 106500
	#define BOOST_TEST_GLOBAL_FIXTURE BOOST_GLOBAL_FIXTURE
#endif

#define HBRS_MPL_REQUIRE_TEST_PASSED                                                                                   \
	BOOST_REQUIRE(                                                                                                     \
		boost::unit_test::results_collector.results(                                                                   \
			boost::unit_test::framework::current_test_case().p_id                                                      \
		).passed()                                                                                                     \
	);

#define HBRS_MPL_TEST_MMEQ(a_dist, b_dist, ucmp)                                                                       \
	{                                                                                                                  \
		using namespace hbrs::mpl;                                                                                     \
		using hbrs::mpl::detail::integral_value;                                                                       \
		using hbrs::mpl::detail::gather;                                                                               \
		namespace mpi = hbrs::mpl::detail::mpi;                                                                        \
		auto a_local = gather(a_dist);                                                                                 \
		auto b_local = gather(b_dist);                                                                                 \
		                                                                                                               \
		if (!mpi::initialized() || (mpi::rank() == 0)) {                                                               \
			auto a_m = (*m)(size(a_local));                                                                            \
			auto b_m = (*m)(size(b_local));                                                                            \
			BOOST_TEST((*equal)(a_m, b_m), "a_m: " << a_m << " b_m: " << b_m);                                         \
		                                                                                                               \
			auto a_n = (*n)(size(a_local));                                                                            \
			auto b_n = (*n)(size(b_local));                                                                            \
		                                                                                                               \
			BOOST_TEST((*equal)(a_n, b_n), "a_n: " << a_n << " b_n: " << b_n);                                         \
		                                                                                                               \
			if ((*equal)(a_m, b_m) && (*equal)(a_n, b_n)) {                                                            \
				for(std::size_t i = 0; i < boost::numeric_cast<std::size_t>(integral_value(a_m)); ++i) {               \
					for(std::size_t j = 0; j < boost::numeric_cast<std::size_t>(integral_value(a_n)); ++j) {           \
						auto x = (*at)(a_local, make_matrix_index(i, j));                                              \
						auto y = (*at)(b_local, make_matrix_index(i, j));                                              \
						if (ucmp) {                                                                                    \
							if constexpr(std::is_signed<std::decay_t<decltype(x)>>::value) {                           \
								x = std::abs(x);                                                                       \
							}                                                                                          \
							if constexpr(std::is_signed<std::decay_t<decltype(y)>>::value) {                           \
								y = std::abs(y);                                                                       \
							}                                                                                          \
						}                                                                                              \
						BOOST_TEST(x == y, "@[" << i << "][" << j << "] := Left: " << x << " Right: " << y);           \
					}                                                                                                  \
				}                                                                                                      \
			}                                                                                                          \
		}                                                                                                              \
	}

#define HBRS_MPL_TEST_VVEQ(a_dist, b_dist, ucmp)                                                                       \
	{                                                                                                                  \
		using namespace hbrs::mpl;                                                                                     \
		using hbrs::mpl::detail::integral_value;                                                                       \
		using hbrs::mpl::detail::gather;                                                                               \
		namespace mpi = hbrs::mpl::detail::mpi;                                                                        \
		auto a_local = gather(a_dist);                                                                                 \
		auto b_local = gather(b_dist);                                                                                 \
		                                                                                                               \
		if (!mpi::initialized() || (mpi::rank() == 0)) {                                                               \
			auto a_sz = (*size)(a_local);                                                                              \
			auto b_sz = (*size)(b_local);                                                                              \
			BOOST_TEST((*equal)(a_sz, b_sz), "a_sz: " << a_sz << " b_sz: " << b_sz);                                   \
		                                                                                                               \
			if ((*equal)(a_sz, b_sz)) {                                                                                \
				for(std::size_t i = 0; i < boost::numeric_cast<std::size_t>(integral_value(a_sz)); ++i) {              \
					auto x = (*at)(a_local, i);                                                                        \
					auto y = (*at)(b_local, i);                                                                        \
					if (ucmp) {                                                                                        \
						if constexpr(std::is_signed<std::decay_t<decltype(x)>>::value) {                               \
							x = std::abs(x);                                                                           \
						}                                                                                              \
						if constexpr(std::is_signed<std::decay_t<decltype(y)>>::value) {                               \
							y = std::abs(y);                                                                           \
						}                                                                                              \
					}                                                                                                  \
					BOOST_TEST(x == y, "@[" << i << "] := Left: " << x << " Right: " << y);                            \
				}                                                                                                      \
			}                                                                                                          \
		}                                                                                                              \
	}

#define HBRS_MPL_TEST_IS_IDENTITY(a_dist)                                                                              \
	{                                                                                                                  \
		using namespace hbrs::mpl;                                                                                     \
		using hbrs::mpl::detail::integral_value;                                                                       \
		using hbrs::mpl::detail::gather;                                                                               \
		namespace mpi = hbrs::mpl::detail::mpi;                                                                        \
		auto a_local = gather(a_dist);                                                                                 \
		if (!mpi::initialized() || (mpi::rank() == 0)) {                                                               \
			auto sz_ = (*size)(a_local);                                                                               \
			auto m_ = (*m)(sz_);                                                                                       \
			auto n_ = (*n)(sz_);                                                                                       \
			                                                                                                           \
			for(std::size_t i = 0; i < boost::numeric_cast<std::size_t>(integral_value(m_)); ++i) {                    \
				for(std::size_t j = 0; j < boost::numeric_cast<std::size_t>(integral_value(n_)); ++j) {                \
					auto x = (*at)(a_local, make_matrix_index(i, j));                                                  \
					if (i == j) {                                                                                      \
						BOOST_TEST(x == 1.0, "@[" << i << "][" << j << "] := " << x << " != 1");                       \
					} else {                                                                                           \
						BOOST_TEST(x == 0.0, "@[" << i << "][" << j << "] := " << x << " != 0");                       \
					}                                                                                                  \
				}                                                                                                      \
			}                                                                                                          \
		}                                                                                                              \
	}

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

struct environment_fixture {
	environment_fixture()
	: env{
		boost::unit_test::framework::master_test_suite().argc,
		boost::unit_test::framework::master_test_suite().argv
	} {};
	
	environment env;
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DETAIL_TEST_IMPL_HPP
