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

#ifndef HBRS_MPL_CORE_IMPLEMENTATIONS_OF_IMPL_HPP
#define HBRS_MPL_CORE_IMPLEMENTATIONS_OF_IMPL_HPP

#include "fwd.hpp"

#include <boost/hana/basic_tuple.hpp>
#include <boost/hana/type.hpp>

#define HBRS_MPL_MAP_FUNCTION_IMPLEMENTATIONS(f_type, f_impls)                                                         \
	HBRS_MPL_NAMESPACE_BEGIN                                                                                           \
	template <>                                                                                                        \
	struct implementations_of_impl<f_type> {                                                                           \
		static constexpr auto                                                                                          \
		apply(f_type) {                                                                                                \
			return f_impls;                                                                                            \
		}                                                                                                              \
	};                                                                                                                 \
	HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_MAP_IMPLS(...)                                                                                        \
	HBRS_MPL_MAP_FUNCTION_IMPLEMENTATIONS(__VA_ARGS__)

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;

HBRS_MPL_DEF_CF1(implementations_of)

template <typename OperationTag, bool Condition>
struct implementations_of_impl<OperationTag, hana::when<Condition>> : hana::default_ {
	template <typename Operation>
	static constexpr hana::basic_tuple<>
	apply(Operation&&) {
		return {};
	}
};

HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_CORE_IMPLEMENTATIONS_OF_IMPL_HPP
