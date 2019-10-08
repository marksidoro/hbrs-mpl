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

#include <boost/test/tree/test_unit.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

environment_fixture::environment_fixture(boost::unit_test::master_test_suite_t & ts)
: env{ts.argc, ts.argv} {}

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
