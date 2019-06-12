/* Copyright (c) 2018-2019 Jakob Meng, <jakobmeng@web.de>
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

#include <hbrs/mpl/detail/environment.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/dt/exception.hpp>
#include <boost/throw_exception.hpp>
#include <algorithm>
#include <iostream>

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
	#include <El.hpp>
#endif

#include <mpi.h>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

struct mpi {
	mpi() {
		setup(nullptr, nullptr);
	}
	
	mpi(int & argc, char** &argv) {
		setup(&argc, &argv);
	}
	
	~mpi() {
		MPI_Finalize();
	}
	
	void
	setup(int * argc, char*** argv) {
		#if defined(HBRS_MPL_ENABLE_ELEMENTAL) && defined(EL_HYBRID)
			/* Ref.: 
			 *  https://github.com/elemental/Elemental/blob/master/src/core/environment.cpp#L150
			 *  https://github.com/elemental/Elemental/blob/master/src/core/imports/mpi.cpp#L69
			 */
			int provided;
			int ec = MPI_Init_thread(argc, argv, MPI_THREAD_MULTIPLE, &provided);
		#else
			int ec = MPI_Init(argc, argv);
		#endif
		
		if (ec != MPI_SUCCESS) {
			BOOST_THROW_EXCEPTION((
				mpi_exception{} 
				<< errinfo_mpi_error_info{mpi_error_info{ec}}
			));
		}
	}
	
};

struct environment::pimpl {
	pimpl()
		: mpi_{}
	#ifdef HBRS_MPL_ENABLE_ELEMENTAL
		, elemental_{}
	#endif
	{};
	
	pimpl(int & argc, char ** &argv)
		: mpi_{argc, argv}
	#ifdef HBRS_MPL_ENABLE_ELEMENTAL
		, elemental_{argc, argv}
	#endif
	{}
	
private:
	mpi mpi_;
#ifdef HBRS_MPL_ENABLE_ELEMENTAL
	El::Environment elemental_;
#endif
};

environment::environment()
: m{std::make_unique<pimpl>()} {}

environment::environment(int & argc, char ** &argv)
: m{std::make_unique<pimpl>(argc, argv)} {}

environment::~environment() {}

environment::environment(environment const& other)
: m{std::make_unique<pimpl>(*other.m)} {}

environment::environment(environment&& other) 
: m{std::make_unique<pimpl>()} 
	/* it's also possible to initialize m with m{}, but then 
	 * the following code will crash: 
	 *  environment a; 
	 *  environment b = std::move(a); 
	 *  a.addOne(); 
	 */
{
	using std::swap;
	swap(*this, other);
}

environment & environment::operator=(environment other) {
	using std::swap;
	swap(*this, other);
	return *this;
}

void environment::swap(environment& other) {
	using std::swap;
	swap(this->m, other.m);
}

void swap(environment& lhs, environment& rhs) {
	lhs.swap(rhs);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END
