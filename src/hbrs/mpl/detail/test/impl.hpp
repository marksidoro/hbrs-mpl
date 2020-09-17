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
#include <hbrs/mpl/fn/absolute.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/equal.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <hbrs/mpl/detail/integral_value.hpp>
#include <boost/test/results_collector.hpp>
#include <boost/test/framework.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>
#include <boost/test/tools/fpc_tolerance.hpp>
#include <boost/test/tools/fpc_op.hpp>
#include <cstdlib>
#include <ostream>
#include <complex>

#ifdef HBRS_MPL_ENABLE_MATLAB
	#include <hbrs/mpl/detail/matlab_cxn.hpp>
#endif // !HBRS_MPL_ENABLE_MATLAB

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
	#include <El.hpp>
#endif // !HBRS_MPL_ENABLE_ELEMENTAL

#ifdef HBRS_MPL_ENABLE_MATLAB
namespace boost {
namespace test_tools {

//TODO: Replaces or document this hack!?!
template<>
inline creal_T&
fpc_tolerance<creal_T>()
{
    static creal_T s_value = {0,0};
	s_value = { fpc_tolerance<real_T>(), fpc_tolerance<real_T>() };
    return s_value;
}

/* namespace test_tools */ }
/* namespace boost */ }

namespace boost {
namespace math {
namespace fpc {

template <>
struct tolerance_based_delegate<creal_T, true> : mpl::true_ {};

/* namespace fpc */ }
/* namespace math */ }
/* namespace boost */ }
#endif // !HBRS_MPL_ENABLE_MATLAB

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
namespace boost {
namespace test_tools {

//TODO: Replaces or document this hack!?!
template<>
inline El::Complex<double>&
fpc_tolerance<El::Complex<double>>()
{
    static El::Complex<double> s_value = {0,0};
	s_value = El::Complex<double>{ fpc_tolerance<double>(), fpc_tolerance<double>() };
    return s_value;
}

/* namespace test_tools */ }
/* namespace boost */ }

namespace boost {
namespace math {
namespace fpc {

template <typename T>
struct tolerance_based_delegate<El::Complex<T>, true> : mpl::true_ {};

/* namespace fpc */ }
/* namespace math */ }
/* namespace boost */ }
#endif // !HBRS_MPL_ENABLE_ELEMENTAL

namespace boost {
namespace test_tools {
namespace assertion {
namespace op {

template <typename FPT, typename Lhs, typename Rhs>
inline assertion_result
compare_complex_fpv(
	Lhs const& lhs,
	Rhs const& rhs,
	op::EQ<Lhs,Rhs>*
) {
	if(lhs == Lhs(0,0)) {
		return
			compare_fpv_near_zero( rhs.real(), (op::EQ<Lhs,Rhs>*)nullptr ) &&
			compare_fpv_near_zero( rhs.imag(), (op::EQ<Lhs,Rhs>*)nullptr );
	} else if (rhs == Rhs(0,0)) {
		return
			compare_fpv_near_zero( lhs.real(), (op::EQ<Lhs,Rhs>*)nullptr ) &&
			compare_fpv_near_zero( lhs.imag(), (op::EQ<Lhs,Rhs>*)nullptr );
	} else {
		fpc::close_at_tolerance<FPT> P1( fpc_tolerance<FPT>(), fpc::FPC_STRONG );
		fpc::close_at_tolerance<FPT> P2( fpc_tolerance<FPT>(), fpc::FPC_STRONG );
		bool p1v = P1(lhs.real(), rhs.real());
		bool p2v = P2(lhs.imag(), rhs.imag());
		
		assertion_result ar(p1v && p2v);
		if(!ar) {
			ar.message() << "Relative difference exceeds tolerance [";
			
			if (!p1v) {
				ar.message() << P1.tested_rel_diff() << " > " << P1.fraction_tolerance() << " (real)";
			} 
			if (!p1v && !p2v) {
				ar.message() << " && ";
			}
			if (!p2v) {
				ar.message() << P2.tested_rel_diff() << " > " << P2.fraction_tolerance() << " (imag)";
			}
			ar.message() << ']';
		}
		return ar;
	}
}

/* namespace op */ }
/* namespace assertion */ }
/* namespace test_tools */ }
/* namespace boost */ }

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
namespace boost {
namespace test_tools {
namespace assertion {
namespace op {

template<typename FPT>
inline assertion_result
compare_fpv(
	El::Complex<double> const& lhs,
	El::Complex<double> const& rhs,
	op::EQ<El::Complex<double>,El::Complex<double>>*
) {
	return compare_complex_fpv<double, std::complex<double>,std::complex<double>>(
		lhs,
		rhs,
		nullptr
	);
}

/* namespace op */ }
/* namespace assertion */ }
/* namespace test_tools */ }
/* namespace boost */ }
#endif // !HBRS_MPL_ENABLE_ELEMENTAL

#ifdef HBRS_MPL_ENABLE_MATLAB
namespace boost {
namespace test_tools {
namespace assertion {
namespace op {

template<>
struct EQ<creal_T,creal_T> {
public:
    typedef typename common_type<creal_T,creal_T>::type FPT;
    typedef EQ<creal_T,creal_T> OP;

    typedef assertion_result result_type;

    static bool
    eval_direct( creal_T const& lhs, creal_T const& rhs );

    static assertion_result
    eval( creal_T const& lhs, creal_T const& rhs );

    template<typename PrevExprType>
    static void
    report( std::ostream&       ostr,
            PrevExprType const& lhs,
            creal_T const&          rhs );

    static char const* revert();
};

template<typename FPT>
inline assertion_result
compare_fpv(
	creal_T const& lhs,
	creal_T const& rhs,
	op::EQ<creal_T, creal_T>*
) {
	return compare_complex_fpv<double, std::complex<double>,std::complex<double>>(
		to_std_complex(lhs),
		to_std_complex(rhs),
		nullptr
	);
}

template<typename PrevExprType>
void
EQ<creal_T,creal_T>::report(std::ostream& ostr, PrevExprType const& lhs, creal_T const& rhs) {
	lhs.report( ostr );
	ostr << revert() << tt_detail::print_helper( rhs );
}

/* namespace op */ }
/* namespace assertion */ }
/* namespace test_tools */ }
/* namespace boost */ }
#endif // !HBRS_MPL_ENABLE_MATLAB

#if defined(HBRS_MPL_ENABLE_ELEMENTAL) && defined(HBRS_MPL_ENABLE_MATLAB)
namespace boost {
namespace test_tools {
namespace assertion {
namespace op {

template<typename FPT>
inline assertion_result
compare_fpv(
	El::Complex<double> const& lhs,
	creal_T const& rhs,
	op::EQ<El::Complex<double>,creal_T>*
) {
	return compare_complex_fpv<double, std::complex<double>,std::complex<double>>(
		lhs,
		to_std_complex(rhs),
		nullptr
	);
}

template<typename FPT>
inline assertion_result
compare_fpv(
	creal_T const& lhs,
	El::Complex<double> const& rhs,
	op::EQ<creal_T, El::Complex<double>>*
) {
	return compare_complex_fpv<double, std::complex<double>,std::complex<double>>(
		to_std_complex(lhs),
		rhs,
		nullptr
	);
}

/* namespace op */ }
/* namespace assertion */ }
/* namespace test_tools */ }
/* namespace boost */ }
#endif // defined(HBRS_MPL_ENABLE_ELEMENTAL) && defined(HBRS_MPL_ENABLE_MATLAB)

#if BOOST_VERSION < 106500
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
		if (!mpi::initialized() || (mpi::comm_rank() == 0)) {                                                          \
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
							auto x_ = (*absolute)(x);                                                                  \
							auto y_ = (*absolute)(y);                                                                  \
							BOOST_TEST(x_ == y_, "@[" << i << "][" << j << "] := Left: " << x_ << " Right: " << y_);   \
						} else {                                                                                       \
							BOOST_TEST(x  == y,  "@[" << i << "][" << j << "] := Left: " << x  << " Right: " << y );   \
						}                                                                                              \
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
		if (!mpi::initialized() || (mpi::comm_rank() == 0)) {                                                          \
			auto a_sz = (*size)(a_local);                                                                              \
			auto b_sz = (*size)(b_local);                                                                              \
			BOOST_TEST((*equal)(a_sz, b_sz), "a_sz: " << a_sz << " b_sz: " << b_sz);                                   \
		                                                                                                               \
			if ((*equal)(a_sz, b_sz)) {                                                                                \
				for(std::size_t i = 0; i < boost::numeric_cast<std::size_t>(integral_value(a_sz)); ++i) {              \
					auto x = (*at)(a_local, i);                                                                        \
					auto y = (*at)(b_local, i);                                                                        \
					if (ucmp) {                                                                                        \
						auto x_ = (*absolute)(x);                                                                      \
						auto y_ = (*absolute)(y);                                                                      \
						BOOST_TEST(x_ == y_, "@[" << i << "] := Left: " << x_ << " Right: " << y_);                    \
					} else {                                                                                           \
						BOOST_TEST(x  == y,  "@[" << i << "] := Left: " << x  << " Right: " << y );                    \
					}                                                                                                  \
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
		if (!mpi::initialized() || (mpi::comm_rank() == 0)) {                                                          \
			auto sz_ = (*size)(a_local);                                                                               \
			auto m_ = (*m)(sz_);                                                                                       \
			auto n_ = (*n)(sz_);                                                                                       \
			                                                                                                           \
			for(std::size_t i = 0; i < boost::numeric_cast<std::size_t>(integral_value(m_)); ++i) {                    \
				for(std::size_t j = 0; j < boost::numeric_cast<std::size_t>(integral_value(n_)); ++j) {                \
					auto x = (*at)(a_local, make_matrix_index(i, j));                                                  \
					if ((*equal)(i, j)) {                                                                              \
						BOOST_TEST(x == 1., "@[" << i << "][" << j << "] := " << x << " != 1");                        \
					} else {                                                                                           \
						BOOST_TEST(x == 0., "@[" << i << "][" << j << "] := " << x << " != 0");                        \
					}                                                                                                  \
				}                                                                                                      \
			}                                                                                                          \
		}                                                                                                              \
	}

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

struct HBRS_MPL_API environment_fixture {
	environment_fixture(boost::unit_test::master_test_suite_t & ts = boost::unit_test::framework::master_test_suite());
	environment env;
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DETAIL_TEST_IMPL_HPP
