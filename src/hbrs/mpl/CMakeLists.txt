# Copyright (c) 2018 Jakob Meng, <jakobmeng@web.de>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

#################### build ####################

add_library(hbrs_mpl STATIC
    dt/ctsav.cpp
    fuse/std/fn/absolute.cpp
    fuse/std/fn/power.cpp)

get_target_property(hbrs_mpl_TARGET_TYPE hbrs_mpl TYPE)
if (hbrs_mpl_TARGET_TYPE STREQUAL "STATIC_LIBRARY")
    target_compile_definitions(hbrs_mpl INTERFACE HBRS_MPL_STATIC_DEFINE)
endif ()

set_target_properties(hbrs_mpl PROPERTIES
    VERSION ${hbrs-mpl_VERSION}
    SOVERSION 0
    INTERFACE_hbrs_mpl_MAJOR_VERSION ${hbrs-mpl_VERSION})

set_property(TARGET hbrs_mpl
    APPEND PROPERTY COMPATIBLE_INTERFACE_STRING hbrs_mpl_MAJOR_VERSION)

target_include_directories(hbrs_mpl
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/src> # for export.hpp
        $<INSTALL_INTERFACE:${HBRS_MPL_INSTALL_INCLUDEDIR}>)

target_include_directories(hbrs_mpl
    SYSTEM
    PUBLIC ${Boost_INCLUDE_DIRS})

target_link_libraries(hbrs_mpl PUBLIC
    ${Boost_LIBRARIES}
    fuse_lib)

target_link_libraries(fuse_lib INTERFACE hbrs_mpl)

include(GenerateExportHeader)
generate_export_header(hbrs_mpl
    BASE_NAME HBRS_MPL
    EXPORT_FILE_NAME export.hpp)

#################### install files ####################

install(TARGETS hbrs_mpl
    EXPORT hbrs-mpl-targets
    ${HBRS_MPL_INSTALL_TARGETS_DEFAULT_DESTINATIONS})

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/export.hpp"
    DESTINATION ${HBRS_MPL_INSTALL_INCLUDEDIR}
    COMPONENT development)
