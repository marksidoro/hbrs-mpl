# Copyright (c) 2018 Jakob Meng, <jakobmeng@web.de>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

#################### configure files ####################

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/config.hpp"
    @ONLY)

#################### build ####################

add_library(hbrs_mpl STATIC)

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
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/src> # for export.hpp
        $<INSTALL_INTERFACE:${HBRS_MPL_INSTALL_INCLUDEDIR}>)

target_include_directories(hbrs_mpl
    SYSTEM
    PUBLIC ${Boost_INCLUDE_DIRS}
    PUBLIC ${MPI_CXX_INCLUDE_DIRS})

if (HBRS_MPL_ENABLE_ELEMENTAL)
    target_include_directories(hbrs_mpl
        SYSTEM
        PUBLIC ${Elemental_INCLUDE_DIRS})
endif()

target_compile_definitions(hbrs_mpl PUBLIC ${MPI_CXX_COMPILE_DEFINITIONS} BOOST_LOG_DYN_LINK)
if (HBRS_MPL_ENABLE_ELEMENTAL)
    target_compile_definitions(hbrs_mpl PUBLIC ${Elemental_DEFINITIONS})
    # NOTE: Linker and compiler flags are not adopted from Elemental because Elemental might be build with a compiler/linker
    #       flags that might be incompatible to the compiler/linker that is used to build this project. For example 
    #       Elemental is build with compile flag -fcx-fortran-rules when using GCC but this is not known by Clang.
    #       Not adopting those flags isn't a problem as Elemental only sets warning and optimization flags.
    #       Ref.: https://github.com/elemental/Elemental/blob/master/CMakeLists.txt#L297
endif()

set_target_properties(hbrs_mpl PROPERTIES
    COMPILE_FLAGS "${MPI_CXX_COMPILE_OPTIONS}"
    COMPILE_FLAGS "${OpenMP_CXX_FLAGS}"
    LINK_FLAGS "${MPI_CXX_LINK_FLAGS}")

#if (HBRS_MPL_ENABLE_ELEMENTAL)
#    set_target_properties(hbrs_mpl PROPERTIES
#        COMPILE_FLAGS ${Elemental_COMPILE_FLAGS}
#        LINK_FLAGS ${Elemental_LINK_FLAGS})
#endif()

target_link_libraries(hbrs_mpl PUBLIC
    ${Boost_LIBRARIES}
    ${MPI_CXX_LIBRARIES}
    ${OpenMP_CXX_LIBRARIES})

if (HBRS_MPL_ENABLE_ELEMENTAL)
    target_link_libraries(hbrs_mpl PUBLIC
        ${Elemental_LIBRARIES})
endif()

include(GenerateExportHeader)
generate_export_header(hbrs_mpl
    BASE_NAME HBRS_MPL
    EXPORT_FILE_NAME export.hpp)

#################### list the subdirectories ####################

add_subdirectory(core)
add_subdirectory(detail)
add_subdirectory(dt)
add_subdirectory(fn)

#################### install files ####################

install(TARGETS hbrs_mpl
    EXPORT hbrs-mpl-targets
    ${HBRS_MPL_INSTALL_TARGETS_DEFAULT_DESTINATIONS})

file(RELATIVE_PATH hbrs_mpl_includedir "${PROJECT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}")
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/export.hpp"
    "${CMAKE_CURRENT_BINARY_DIR}/config.hpp"
    DESTINATION "${HBRS_MPL_INSTALL_INCLUDEDIR}/${hbrs_mpl_includedir}"
    COMPONENT development)

install(DIRECTORY ./
    DESTINATION "${HBRS_MPL_INSTALL_INCLUDEDIR}/${hbrs_mpl_includedir}"
    COMPONENT development
    FILES_MATCHING PATTERN "*.hpp")
