# Copyright (c) 2018 Jakob Meng, <jakobmeng@web.de>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

#################### fuse ####################

add_library(fuse_lib INTERFACE)

include(FuseLibrary)
list_fuse_headers(fuse_headers
    FILTER "fn/*.hpp"
    PATH "${CMAKE_CURRENT_SOURCE_DIR}")
target_fuse_headers(fuse_lib ${fuse_headers})

fuse_library(fuse_lib
    PP_PREFIX "HBRS_MPL_FUSE"
    DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/fuse")

target_fuse_tags(fuse_lib
    "HBRS_MPL_FUSE_HBRS_MPL"
    "HBRS_MPL_FUSE_BOOST_HANA"
    "HBRS_MPL_FUSE_BOOST_RANGE"
    "HBRS_MPL_FUSE_STD")

target_fuse_directories(fuse_lib "${CMAKE_CURRENT_SOURCE_DIR}/fuse/hbrs/mpl/"    INCLUDE_PREFIX "hbrs/mpl/fuse/hbrs/mpl")
target_fuse_directories(fuse_lib "${CMAKE_CURRENT_SOURCE_DIR}/fuse/boost/hana/"  INCLUDE_PREFIX "hbrs/mpl/fuse/boost/hana")
target_fuse_directories(fuse_lib "${CMAKE_CURRENT_SOURCE_DIR}/fuse/boost/range/" INCLUDE_PREFIX "hbrs/mpl/fuse/boost/range")
target_fuse_directories(fuse_lib "${CMAKE_CURRENT_SOURCE_DIR}/fuse/std/"         INCLUDE_PREFIX "hbrs/mpl/fuse/std")

#################### configure files ####################

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/config.hpp"
    @ONLY)

#################### install files ####################

install(TARGETS fuse_lib
    EXPORT hbrs-mpl-targets
    ${HBRS_MPL_INSTALL_TARGETS_DEFAULT_DESTINATIONS})

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/config.hpp
    DESTINATION "${HBRS_MPL_INSTALL_INCLUDEDIR}"
    COMPONENT development)

install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/fuse/"
    DESTINATION "${HBRS_MPL_INSTALL_INCLUDEDIR}/fuse"
    COMPONENT development
    FILES_MATCHING PATTERN "*.hpp")

install(DIRECTORY ./
    DESTINATION "${HBRS_MPL_INSTALL_INCLUDEDIR}"
    COMPONENT development
    FILES_MATCHING PATTERN "*.hpp")
