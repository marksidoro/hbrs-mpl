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

#include "impl.hpp"

#include <hbrs/mpl/detail/log.hpp>
#include <boost/log/expressions.hpp>

#include <boost/test/tree/test_unit.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_parameters.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

environment_fixture::environment_fixture(boost::unit_test::master_test_suite_t & ts)
: env{ts.argc, ts.argv} {
	namespace utf = boost::unit_test;
	
	utf::log_level log_lvl = utf::runtime_config::get<utf::log_level>(utf::runtime_config::btrt_log_level);

	// Ref.: https://github.com/boostorg/test/blob/develop/include/boost/test/detail/log_level.hpp
	// enum BOOST_TEST_ENUM_SYMBOL_VISIBLE log_level {
	//     invalid_log_level        = -1,
	//     log_successful_tests     = 0,
	//     log_test_units           = 1,
	//     log_messages             = 2,
	//     log_warnings             = 3,
	//     log_all_errors           = 4, // reported by unit test macros
	//     log_cpp_exception_errors = 5, // uncaught C++ exceptions
	//     log_system_errors        = 6, // including timeouts, signals, traps
	//     log_fatal_errors         = 7, // including unit test macros or
	//                                   // fatal system errors
	//     log_nothing              = 8
	// };
	
	// Ref.: https://github.com/boostorg/log/blob/develop/include/boost/log/trivial.hpp
	// enum severity_level
	// {
	//     trace,
	//     debug,
	//     info,
	//     warning,
	//     error,
	//     fatal
	// };
	
	log::trivial::severity_level sev_lvl = {};
	switch(log_lvl) {
		case utf::log_successful_tests:
			sev_lvl = log::trivial::trace;
			break;
		case utf::log_test_units:
			sev_lvl = log::trivial::debug;
			break;
		case utf::log_messages:
			sev_lvl = log::trivial::info;
			break;
		case utf::log_warnings:
			sev_lvl = log::trivial::warning;
			break;
		case utf::log_all_errors:
		case utf::log_cpp_exception_errors:
		case utf::log_system_errors:
			sev_lvl = log::trivial::error;
			break;
		case utf::log_fatal_errors:
		case utf::log_nothing:
			sev_lvl = log::trivial::fatal;
			break;
		
		default:
			sev_lvl = log::trivial::info;
			break;
	}
	
	log::core::get()->set_filter(log::trivial::severity >= sev_lvl);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#ifdef HBRS_MPL_ENABLE_MATLAB
namespace boost {
namespace test_tools {
namespace assertion {
namespace op {

bool
EQ<creal_T,creal_T>::eval_direct( creal_T const& lhs, creal_T const& rhs ) {
	return lhs == rhs;
}

assertion_result
EQ<creal_T,creal_T>::eval( creal_T const& lhs, creal_T const& rhs ) {
	if( fpc_tolerance<FPT>() == FPT{0} ) {
		return eval_direct( lhs, rhs );
	}
	return compare_fpv<FPT>( lhs, rhs, (OP*)0 );
}

char const*
EQ<creal_T,creal_T>::revert() {
	return " != ";
}

/* namespace op */ }
/* namespace assertion */ }
/* namespace test_tools */ }
/* namespace boost */ }
#endif // !HBRS_MPL_ENABLE_MATLAB
