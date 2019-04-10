# Copyright (c) 2018 Jakob Meng, <jakobmeng@web.de>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

#################### build ####################

add_library(elemental STATIC
    fuse/fn/absolute.cpp
    fuse/fn/at.cpp
    fuse/fn/columns.cpp
    fuse/fn/diag.cpp
    fuse/fn/divide.cpp
    fuse/fn/expand.cpp
    fuse/fn/fold1.cpp
    fuse/fn/m.cpp
    fuse/fn/mean.cpp
    fuse/fn/minus.cpp
    fuse/fn/multiply.cpp
    fuse/fn/n.cpp
    fuse/fn/pca.cpp
    fuse/fn/pca_filter.cpp
    fuse/fn/plus.cpp
    fuse/fn/power.cpp
    fuse/fn/rows.cpp
    fuse/fn/select.cpp
    fuse/fn/size.cpp
    fuse/fn/svd.cpp
    fuse/fn/times.cpp
    fuse/fn/transform.cpp
    fuse/fn/transpose.cpp
    fuse/fn/vertcat.cpp
    fuse/fn/zip.cpp)

target_include_directories(elemental
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<INSTALL_INTERFACE:${HBRS_MPL_INSTALL_ELEMENTAL_INCLUDEDIR}/..>)

target_include_directories(elemental
    SYSTEM
    PUBLIC ${Boost_INCLUDE_DIRS}
    PUBLIC ${Elemental_INCLUDE_DIRS}
    PUBLIC ${MPI_CXX_INCLUDE_DIRS})

target_compile_definitions(elemental PUBLIC ${Elemental_DEFINITIONS} ${MPI_CXX_COMPILE_DEFINITIONS})
# NOTE: Linker and compiler flags are not adopted from Elemental because Elemental might be build with a compiler/linker
#       flags that might be incompatible to the compiler/linker that is used to build this project. For example 
#       Elemental is build with compile flag -fcx-fortran-rules when using GCC but this is not known by Clang.
#       Not adopting those flags isn't a problem as Elemental only sets warning and optimization flags.
#       Ref.: https://github.com/elemental/Elemental/blob/master/CMakeLists.txt#L297
set_target_properties(elemental PROPERTIES
    COMPILE_FLAGS "${MPI_CXX_COMPILE_OPTIONS}"
    LINK_FLAGS "${MPI_CXX_LINK_FLAGS}"
    COMPILE_FLAGS "${OpenMP_CXX_FLAGS}"
#   COMPILE_FLAGS ${Elemental_COMPILE_FLAGS}
#   LINK_FLAGS ${Elemental_LINK_FLAGS}
)

target_link_libraries(elemental PUBLIC
    ${Boost_LIBRARIES}
    ${Elemental_LIBRARIES}
    ${MPI_CXX_LIBRARIES}
    ${OpenMP_CXX_LIBRARIES}
    fuse_lib)

target_link_libraries(fuse_lib INTERFACE elemental)

#################### install files ####################

install(TARGETS elemental
    EXPORT hbrs-mpl-targets
    ${HBRS_MPL_INSTALL_TARGETS_DEFAULT_DESTINATIONS})
