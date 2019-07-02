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

#ifndef HBRS_MPL_FN_AND_IMPL_HPP
#define HBRS_MPL_FN_AND_IMPL_HPP

#include "fwd.hpp"

#include <hbrs/mpl/dt/function.hpp>
#include <hbrs/mpl/core/annotation.hpp>

HBRS_MPL_NAMESPACE_BEGIN

template <typename Expr1, typename Expr2>
constexpr decltype(auto)
and_t::operator()(Expr1 && e1, Expr2 && e2) const {
	return call(
		and_, 
		HBRS_MPL_FWD(e1),
		add_annotation(HBRS_MPL_FWD(e2), delay_evaluation{})
	);
}

constexpr decltype(auto)
and_t::operator*() const {
	return boost::hana::demux(evaluate)(and_);
}

HBRS_MPL_NAMESPACE_END

#include "impl/boost_hana.hpp"
#include "impl/hbrs_mpl.hpp"
#include "impl/std.hpp"

#endif // !HBRS_MPL_FN_AND_IMPL_HPP
