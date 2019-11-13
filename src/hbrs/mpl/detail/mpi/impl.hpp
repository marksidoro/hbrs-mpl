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

#ifndef HBRS_MPL_DETAIL_MPI_IMPL_HPP
#define HBRS_MPL_DETAIL_MPI_IMPL_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <mpi.h>
#include <cstdlib>
#include <boost/hana/type.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {
namespace mpi {

MPI_Datatype
datatype(hana::basic_type<double>);
MPI_Datatype
datatype(hana::basic_type<int>);
MPI_Datatype
datatype(hana::basic_type<unsigned long>);
MPI_Datatype
datatype(hana::basic_type<unsigned long long>);

bool
initialized();

int
comm_rank(MPI_Comm comm = MPI_COMM_WORLD);

int
comm_size(MPI_Comm comm = MPI_COMM_WORLD);

void
barrier(MPI_Comm comm = MPI_COMM_WORLD);

void
abort(MPI_Comm comm = MPI_COMM_WORLD, int errorcode = EXIT_FAILURE);

MPI_Request
ibcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm);

template<typename T>
MPI_Request
ibcast(T *buffer, int count, int root, MPI_Comm comm) {
	return ibcast(buffer, count, datatype(hana::type_c<T>), root, comm);
}

MPI_Status
wait(MPI_Request & request);

MPI_Status
probe(int source, int tag, MPI_Comm comm);

int
get_count(MPI_Status const& status, MPI_Datatype datatype);

MPI_Request
isend(void const* buffer, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);

template<typename T>
MPI_Request
isend(T const* buffer, int count, int dest, int tag, MPI_Comm comm) {
	return isend(buffer, count, datatype(hana::type_c<T>), dest, tag, comm);
}

MPI_Request
irecv(void *buffer, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm);

template<typename T>
MPI_Request
irecv(T *buffer, int count, int source, int tag, MPI_Comm comm) {
	return irecv(buffer, count, datatype(hana::type_c<T>), source, tag, comm);
}

void
allreduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);

template<typename T>
void
allreduce(T const* sendbuf, T* recvbuf, int count, MPI_Op op, MPI_Comm comm) {
	return allreduce(sendbuf, recvbuf, count, datatype(hana::type_c<T>), op, comm);
}

MPI_Request
iallreduce(void const *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);

template<typename T>
MPI_Request
iallreduce(T const *sendbuf, T *recvbuf, int count, MPI_Op op, MPI_Comm comm) {
	return iallreduce(sendbuf, recvbuf, count, datatype(hana::type_c<T>), op, comm);
}

void
allgather(void const* sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm);

template<typename S, typename R>
void
allgather(S const* sendbuf, int sendcount, R *recvbuf, int recvcount, MPI_Comm comm) {
	return allgather(sendbuf, sendcount, datatype(hana::type_c<S>), recvbuf, recvcount, datatype(hana::type_c<R>), comm);
}

MPI_Request
iallgather(void const* sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm);

template<typename S, typename R>
MPI_Request
iallgather(S const* sendbuf, int sendcount, R *recvbuf, int recvcount, MPI_Comm comm) {
	return iallgather(sendbuf, sendcount, datatype(hana::type_c<S>), recvbuf, recvcount, datatype(hana::type_c<R>), comm);
}

/* namespace mpi */ }
/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DETAIL_MPI_IMPL_HPP
