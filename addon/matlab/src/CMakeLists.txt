# Copyright (c) 2018 Jakob Meng, <jakobmeng@web.de>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

#################### build ####################

add_library(matlab STATIC
    dt/matlab_cxn.cpp
    fuse/fn/at.cpp
    fuse/fn/bidiag.cpp
    fuse/fn/columns.cpp
    fuse/fn/expand.cpp
    fuse/fn/m.cpp
    fuse/fn/mean.cpp
    fuse/fn/multiply.cpp
    fuse/fn/n.cpp
    fuse/fn/pca.cpp
    fuse/fn/pca_filter.cpp
    fuse/fn/plus.cpp
    fuse/fn/rows.cpp
    fuse/fn/select.cpp
    fuse/fn/svd.cpp
    fuse/fn/transpose.cpp
    fuse/fn/vertcat.cpp)

target_include_directories(matlab
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../..>
        $<INSTALL_INTERFACE:${HBRS_MPL_INSTALL_MATLAB_INCLUDEDIR}/..>)

target_include_directories(matlab
    SYSTEM
    PUBLIC ${Boost_INCLUDE_DIRS})

target_link_libraries(matlab PUBLIC ${Boost_LIBRARIES} fuse_lib)
target_link_libraries(matlab INTERFACE matlab_cxn)
target_link_libraries(fuse_lib INTERFACE matlab)

#################### install files ####################

install(TARGETS matlab
    EXPORT hbrs-mpl-targets
    ${HBRS_MPL_INSTALL_TARGETS_DEFAULT_DESTINATIONS})
