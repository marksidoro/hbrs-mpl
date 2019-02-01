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

#pragma once

#ifndef HBRS_MPL_DETAIL_ENVIRONMENT_HPP
#define HBRS_MPL_DETAIL_ENVIRONMENT_HPP

#include <hbrs/mpl/config.hpp>
#include <memory>
#include <experimental/propagate_const>
#include <mpi.h>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

struct /*HBRS_MPL_API*/ environment {
	environment();
	environment(int & argc, char ** &argv);
	virtual ~environment();

	environment(environment const& other);
	environment & operator=(environment other);
	environment(environment&& other);
	
	void swap(environment& other);
	friend void swap(environment& lhs, environment& rhs);
private:
	struct pimpl;
	//TODO: Replace std::experimental::propagate_const?!
	std::experimental::propagate_const<std::unique_ptr<pimpl>> m;
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DETAIL_ENVIRONMENT_HPP
