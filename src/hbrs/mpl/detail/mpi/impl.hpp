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
#include <boost/config.hpp> 
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {
namespace mpi {

/* The MPI standard [1] defines two operations MPI_MAXLOC and MPI_MINLOC which
 * take a datatype argument that represents a pair (value and index):
 * 
 * Name                | Description
 * --------------------|---------------------
 * MPI_FLOAT_INT       | float and int
 * MPI_DOUBLE_INT      | double and int
 * MPI_LONG_INT        | long and int
 * MPI_2INT            | pair of int
 * MPI_SHORT_INT       | short and int
 * MPI_LONG_DOUBLE_INT | long double and int
 *
 * "The datatype MPI_FLOAT_INT is as if defined by the following sequence of instructions.
 *   type[0] = MPI_FLOAT
 *   type[1] = MPI_INT
 *   disp[0] = 0
 *   disp[1] = sizeof(float)
 *   block[0] = 1
 *   block[1] = 1
 *   MPI_TYPE_CREATE_STRUCT(2, block, disp, type, MPI_FLOAT_INT)
 *  Similar statements apply for MPI_LONG_INT and MPI_DOUBLE_INT." [1], p.180
 *
 * [1], p.181 and [2] show how MPI_DOUBLE_INT can be implemented as a C struct:
 *   struct {
 *       double val;
 *       int   rank;
 *   } in[30], out[30];
 *
 *
 * The standard definition of MPI_FLOAT_INT above implies no padding between float
 * and int. The C struct decl. is only MPI-compatible if there is a guarantee
 * that no padding space is inserted between val and rank [3].
 *
 * Additionally the standard (probably) asserts proper alignment of MPI_*_INT [4]:
 * "Example 4.1 Assume that Type = {(double, 0), (char, 8)} (a double at displacement
 *  zero, followed by a char at displacement eight). Assume, furthermore, that doubles have
 *  to be strictly aligned at addresses that are multiples of eight. Then, the extent of this
 *  datatype is 16 (9 rounded to the next multiple of 8). A datatype that consists of a character
 *  immediately followed by a double will also have an extent of 16." [1], p.85
 *
 * On x86-64 linux, gcc 9.2 [5], clang 9.0.0 [6] and icc 19.0.1 [7] produce C struct's
 * compatible to MPI's datatype definition, without padding and with proper alignment:
 *
 *   #include <memory>
 *   #include <iterator>
 *   #include <cassert>
 *
 *   int main() {
 *       struct pair {
 *           double d;
 *           int i;
 *       };
 *       static_assert(sizeof(double) == 8, "");
 *       static_assert(sizeof(int) == 4, "");
 *       static_assert(sizeof(double)+sizeof(int) == 12, "");
 *       static_assert(sizeof(pair) == 16, "");
 *
 *       static constexpr pair p {};
 *
 *       assert(
 *           (std::ptrdiff_t)std::addressof(p.d)-
 *           (std::ptrdiff_t)std::addressof(p)
 *           == 0);
 *
 *       assert(
 *           (std::ptrdiff_t)std::addressof(p.i)-
 *           (std::ptrdiff_t)std::addressof(p)
 *           == sizeof(double)
 *       );
 *
 *       static_assert(
 *           (sizeof(double)+sizeof(int))%alignof(double) == 4
 *       , "");
 *
 *       static_assert(alignof(pair) == alignof(double), "");
 *
 *       #ifdef NDEBUG
 *           return 1;
 *       #else
 *           return 0;
 *       #endif
 *   }
 * 
 * Padding might be an issue on systems where e.g. int is 64-bit and has to be aligned
 * on 8 bytes. One might need to instruct the compiler to generate packed C struct's
 * to avoid padding [3]:
 *
 *   #pragma pack(push, 1)
 *   struct pair {
 *       double d;
 *       int i;
 *   };
 *   #pragma pack(pop)
 *
 * The size of this packed C struct is 12 bytes on x86-64 linux with gcc 9.2 [9],
 * clang 9.0.0 [10] and icc 19.0.1 [11], which is not correctly aligned:
 *
 *   int main() {
 *       #pragma pack(push, 1)
 *       struct pair {
 *           double d;
 *           int i;
 *       };
 *       #pragma pack(pop)
 *
 *       static_assert(sizeof(double) == 8, "");
 *       static_assert(sizeof(int) == 4, "");
 *       static_assert(sizeof(double)+sizeof(int) == 12, "");
 *       static_assert(sizeof(pair) == 12, "");
 *       static_assert(alignof(double) == 8, "");
 *       static_assert(alignof(pair) == 1, "");
 *
 *       return 0;
 *   }
 *
 * Alignment is fixed on gcc 9.2 [12], clang 9.0.0 [13] and icc 19.0.1 [14]
 * by applying the C++11 alignas keyword to the C struct:
 *
 *   int main() {
 *      #pragma pack(push, 1)
 *      struct alignas(alignof(double)) pair {
 *          double d;
 *          int i;
 *      };
 *      #pragma pack(pop)
 *
 *      static_assert(sizeof(double) == 8, "");
 *      static_assert(sizeof(int) == 4, "");
 *      static_assert(sizeof(double)+sizeof(int) == 12, "");
 *      static_assert(sizeof(pair) == 16, "");
 *      static_assert(alignof(double) == 8, "");
 *      static_assert(alignof(pair) == 8, "");
 *
 *      return 0;
 *  }
 *
 * NOTE: OpenMPI is (probably) able to handle non-packed structs a.k.a. structs with padding
 *       between float/double and int, during datatype definition in [8], lines 17-20:
 *
 *        1 #define DECLARE_MPI2_COMPOSED_STRUCT_DDT( PDATA, MPIDDT, MPIDDTNAME, type1, type2, MPIType1, MPIType2, FLAGS) \
 *        2     do {                                                                             \
 *        3         struct { type1 v1; type2 v2; } s[2];                                         \
 *        4         ompi_datatype_t *types[2], *ptype;                                           \
 *        5         int bLength[2] = {1, 1};                                                     \
 *        6         ptrdiff_t base, displ[2];                                                    \
 *        7                                                                                      \
 *        8         types[0] = (ompi_datatype_t*)ompi_datatype_basicDatatypes[MPIType1];         \
 *        9         types[1] = (ompi_datatype_t*)ompi_datatype_basicDatatypes[MPIType2];         \
 *       10         base = (ptrdiff_t)(&(s[0]));                                                 \
 *       11         displ[0] = (ptrdiff_t)(&(s[0].v1));                                          \
 *       12         displ[0] -= base;                                                            \
 *       13         displ[1] = (ptrdiff_t)(&(s[0].v2));                                          \
 *       14         displ[1] -= base;                                                            \
 *       15                                                                                      \
 *       16         ompi_datatype_create_struct( 2, bLength, displ, types, &ptype );             \
 *       17         displ[0] = (ptrdiff_t)(&(s[1]));                                             \
 *       18         displ[0] -= base;                                                            \
 *       19         if( displ[0] != (displ[1] + (ptrdiff_t)sizeof(type2)) )                      \
 *       20             ptype->super.ub = displ[0];  // force a new extent for the datatype      \
 *       21         ptype->super.flags |= (FLAGS);                                               \
 *       22         ptype->id = MPIDDT;                                                          \
 *       23         ompi_datatype_commit( &ptype );                                              \
 *       24         COPY_DATA_DESC( PDATA, ptype );                                              \
 *       25         (PDATA)->super.flags &= ~OPAL_DATATYPE_FLAG_PREDEFINED;                      \
 *       26         (PDATA)->super.flags |= OMPI_DATATYPE_FLAG_PREDEFINED |                      \
 *       27                                 OMPI_DATATYPE_FLAG_ANALYZED   |                      \
 *       28                                 OMPI_DATATYPE_FLAG_MONOTONIC;                        \
 *       29         ptype->super.desc.desc = NULL;                                               \
 *       30         ptype->super.opt_desc.desc = NULL;                                           \
 *       31         OBJ_RELEASE( ptype );                                                        \
 *       32         opal_string_copy( (PDATA)->name, MPIDDTNAME, MPI_MAX_OBJECT_NAME );          \
 *       33     } while(0)
 *       34
 *       35 DECLARE_MPI2_COMPOSED_STRUCT_DDT( &ompi_mpi_float_int.dt, OMPI_DATATYPE_FLOAT_INT, "MPI_FLOAT_INT",
 *       36                                   float, int,
 *       37                                   OMPI_DATATYPE_MPI_FLOAT, OMPI_DATATYPE_MPI_INT,
 *       38                                   OMPI_DATATYPE_FLAG_DATA_C );
 *
 * Ref.:
 *  [1] https://www.mpi-forum.org/docs/mpi-3.1/mpi31-report.pdf
 *  [2] https://dyn.manpages.debian.org/buster/openmpi-doc/MPI_Reduce.openmpi.3.en.html
 *  [3] https://stackoverflow.com/a/44508948
 *  [4] https://stackoverflow.com/a/51956431
 *  [5] https://gcc.godbolt.org/z/Cvq5G7
 *  [6] https://gcc.godbolt.org/z/gL9y6N
 *  [7] https://gcc.godbolt.org/z/ebardj
 *  [8] https://github.com/open-mpi/ompi/blob/master/ompi/datatype/ompi_datatype_module.c
 *  [9] https://gcc.godbolt.org/z/239vBP
 * [10] https://gcc.godbolt.org/z/m4guVL
 * [11] https://gcc.godbolt.org/z/5PzG55
 * [12] https://gcc.godbolt.org/z/_ZopRg
 * [13] https://gcc.godbolt.org/z/WUhDTq
 * [14] https://gcc.godbolt.org/z/3-N-xG
 */

#if !defined(BOOST_GCC) && !defined(BOOST_CLANG) && !defined(BOOST_MSVC) && !defined(BOOST_INTEL)
	#pragma message "Make sure that your compiler does support #pragma pack!"
#endif

#pragma pack(push, 1)
template<typename T1, typename T2>
struct alignas(sizeof(T1)) pair{
	T1 first;
	T2 second;
};
#pragma pack(pop)

MPI_Datatype
datatype(hana::basic_type<double>);
MPI_Datatype
datatype(hana::basic_type<int>);
MPI_Datatype
datatype(hana::basic_type<unsigned long>);
MPI_Datatype
datatype(hana::basic_type<unsigned long long>);
MPI_Datatype
datatype(hana::basic_type<pair<int,int>>);
MPI_Datatype
datatype(hana::basic_type<pair<short,int>>);
MPI_Datatype
datatype(hana::basic_type<pair<long,int>>);
MPI_Datatype
datatype(hana::basic_type<pair<float,int>>);
MPI_Datatype
datatype(hana::basic_type<pair<double,int>>);

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
