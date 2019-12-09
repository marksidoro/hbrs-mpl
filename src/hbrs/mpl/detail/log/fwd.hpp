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

#ifndef HBRS_MPL_DETAIL_LOG_FWD_HPP
#define HBRS_MPL_DETAIL_LOG_FWD_HPP

#include <hbrs/mpl/config.hpp>
#include <boost/log/trivial.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace log = boost::log;
namespace detail {

template<typename T>
struct loggable;

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DETAIL_LOG_FWD_HPP