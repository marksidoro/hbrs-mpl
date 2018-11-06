# Copyright (c) 2016-2018 Jakob Meng, <jakobmeng@web.de>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

if (NOT MATLAB_FOUND)
    message(FATAL_ERROR "MATLAB not found. Required dependency of MATLAB adapter.")
endif()

#################### build ####################

# Example for downloading a MATLAB function from MATLAB Central File Exchange,
# https://de.mathworks.com/matlabcentral/fileexchange/
#include (ExternalProject)
#ExternalProject_Add(matlab_bidiag_m
#    PREFIX            external
#    URL               https://de.mathworks.com/matlabcentral/mlc-downloads/downloads/e5467b3e-4a80-11e4-9553-005056977bd0/7b5b23f4-aadf-9513-306b-cd59afb6e4c9/packages/zip
#    URL_HASH          SHA256=234ec62520324da3c225d2f4392e9fdf603796694415252bb80b22617e2338a3
#    DOWNLOAD_NAME     bidiag.zip
#    EXCLUDE_FROM_ALL  TRUE
#    CONFIGURE_COMMAND ""
#    BUILD_COMMAND     ${CMAKE_COMMAND} -E copy_if_different "../matlab_bidiag_m/bidiag.m" "../../../"
#    INSTALL_COMMAND   "")

# MATLAB requires a LAPACK library and a LAPACKE C header file to use LAPACK calls in code generated from MATLAB code.
# Ref.: https://de.mathworks.com/help/coder/ref/coder.lapackcallback-class.html
if (NOT LAPACK_FOUND)
    message(WARNING "LAPACK library not found, MATLAB code generation will not utilize LAPACK.")
elseif (NOT LAPACKE_FOUND)
    message(WARNING "LAPACKE library not found, MATLAB code generation will not utilize LAPACK.")
else()
    set(_LAPACK_LIBRARIES)
    set(_LAPACKE_INCLUDE_DIRS)

    foreach(_inc ${LAPACKE_INCLUDE_DIRS})
        if("${_LAPACKE_INCLUDE_DIRS}" STREQUAL "")
            set(_LAPACKE_INCLUDE_DIRS "'${_inc}'")
        else()
            set(_LAPACKE_INCLUDE_DIRS "${_LAPACKE_INCLUDE_DIRS},'${_inc}'")
        endif()
    endforeach()

    foreach(_lib ${LAPACK_LIBRARIES})
        if("${_LAPACK_LIBRARIES}" STREQUAL "")
            set(_LAPACK_LIBRARIES "'${_lib}'")
        else()
            set(_LAPACK_LIBRARIES "${_LAPACK_LIBRARIES},'${_lib}'")
        endif()
    endforeach()

    configure_file(lapack_callback.m.in "${CMAKE_CURRENT_BINARY_DIR}/lapack_callback.m" @ONLY)
endif()

# Make local copy of 'wnanmean' as a workaround because MATLAB Coder does not support standalone code generation for it.
file(COPY "${Matlab_ROOT_DIR}/toolbox/stats/classreg/+classreg/+learning/+internal/wnanmean.m"
    DESTINATION "${CMAKE_CURRENT_BINARY_DIR}"
    NO_SOURCE_PERMISSIONS)

set(matlab_cxn_LIBRARY_FILENAME "${CMAKE_STATIC_LIBRARY_PREFIX}matlab_cxn${CMAKE_STATIC_LIBRARY_SUFFIX}")
set(matlab_cxn_LIBRARY_BUILD_PATH   "${CMAKE_CURRENT_BINARY_DIR}/${matlab_cxn_LIBRARY_FILENAME}")
# use an absolute library path for target property INTERFACE_LINK_LIBRARIES to be able to find library in install path
set(matlab_cxn_LIBRARY_INSTALL_PATH "${CMAKE_INSTALL_PREFIX}/${HBRS_MPL_INSTALL_LIBDIR}/${matlab_cxn_LIBRARY_FILENAME}")

add_custom_command(
    OUTPUT "${matlab_cxn_LIBRARY_BUILD_PATH}"
    PRE_BUILD
    COMMAND ${Matlab_MAIN_PROGRAM} -nojvm -nodisplay -nosplash -nodesktop -r                 "\
try;                                                                                          \
  cd ${CMAKE_CURRENT_SOURCE_DIR}/;                                                            \
  addpath('${CMAKE_CURRENT_BINARY_DIR}');                                                     \
  build('${CMAKE_CURRENT_BINARY_DIR}', '${CMAKE_CURRENT_SOURCE_DIR}', '${CMAKE_BUILD_TYPE}'); \
  exit(0);                                                                                    \
catch E;                                                                                      \
  disp(getReport(E));                                                                         \
  exit(255);                                                                                  \
end;                                                                                          \
"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "matlab_cxn${CMAKE_STATIC_LIBRARY_SUFFIX}" "${matlab_cxn_LIBRARY_FILENAME}"
    # static libraries might have a prefix by convention so create a prefixed copy if neccessary
    VERBATIM)

add_custom_target(matlab_codegen DEPENDS "${matlab_cxn_LIBRARY_BUILD_PATH}")

add_library(matlab_cxn INTERFACE)
# We use "INTERFACE" instead of "STATIC IMPORTED GLOBAL" because imported libraries are not installable [1].
# Because INCLUDE_* properties are exported even when using target_link_libraries(<target> PRIVATE <imported-library>)
# we have to install matlab_cxn if any other library depends on it [2].
# Ref.: 
#  [1] https://gitlab.kitware.com/cmake/cmake/issues/14311
#  [2] https://gitlab.kitware.com/cmake/cmake/issues/18049

add_dependencies(matlab_cxn matlab_codegen)

target_link_libraries(matlab_cxn INTERFACE
    $<BUILD_INTERFACE:${matlab_cxn_LIBRARY_BUILD_PATH}>
    $<INSTALL_INTERFACE:${matlab_cxn_LIBRARY_INSTALL_PATH}>)

target_include_directories(matlab_cxn
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>/../..
        $<INSTALL_INTERFACE:${HBRS_MPL_INSTALL_MATLAB_CXN_INCLUDEDIR}/../..>)

target_include_directories(matlab_cxn
    SYSTEM
    INTERFACE ${Matlab_INCLUDE_DIRS})

#################### install files ####################

install(FILES ${matlab_cxn_LIBRARY_BUILD_PATH}
    DESTINATION ${HBRS_MPL_INSTALL_LIBDIR})

install(TARGETS matlab_cxn
    EXPORT hbrs-mpl-targets
    ${HBRS_MPL_INSTALL_TARGETS_DEFAULT_DESTINATIONS})

install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/"
    DESTINATION "${HBRS_MPL_INSTALL_MATLAB_CXN_INCLUDEDIR}"
    COMPONENT development
    FILES_MATCHING PATTERN "*.h")
