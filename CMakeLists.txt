# Copyright (c) 2016-2018 Jakob Meng, <jakobmeng@web.de>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

#################### project settings ####################

project(hbrs-mpl VERSION 2019.0.0.0)

include(FeatureSummary)

#################### options ####################

option(HBRS_MPL_ENABLE_ADDON_ELEMENTAL        "Build elemental adapter." ON)
option(HBRS_MPL_ENABLE_ADDON_MATLAB           "Build matlab adapter." ON)

option(HBRS_MPL_ENABLE_OBJDUMP                "Disassemble executables." OFF)
option(HBRS_MPL_ENABLE_MEMCHECK               "Run tests and examples with valgrind." OFF)
option(HBRS_MPL_ENABLE_TESTS                  "Build unit tests." OFF)

#################### find all used packages ####################

# NOTE: Keep list of packages in sync with dependencies in *-cmake.in!

find_package(hbrs-cmake REQUIRED)
set_package_properties(hbrs-cmake PROPERTIES
    PURPOSE "Required for CMake modules."
    TYPE REQUIRED
)

# Hana is part of boost since 1.61, Ref.: http://www.boost.org/users/history/
# BOOST_TEST macro requires 1.59, Ref.: https://stackoverflow.com/a/38083784/6490710
# boost::hana::overload_linearly with single argument is supported since boost 1.62, Ref.: https://github.com/boostorg/hana/commit/89755947faffc56ff76ef3eecca0430191e976b8
find_package(Boost 1.62 REQUIRED COMPONENTS unit_test_framework system thread serialization chrono program_options filesystem iostreams regex exception signals)
set_package_properties(Boost PROPERTIES
    PURPOSE "Required for meta programming, unit tests and others."
    TYPE REQUIRED
)

find_package(LAPACK)
set_package_properties(LAPACK PROPERTIES
    PURPOSE "Optional for matlab adapter."
)

# NOTE: If find_package(LAPACK) does not choose LAPACKE but another LAPACK library like e.g. OpenBLAS then you have to 
#       use static LAPACKE library (liblapacke.a) instead of dynamic LAPACKE library (liblapacke.so) because the 
#       dynamic library requires symbols that are missing from other LAPACK libraries. To find the static LAPACKE 
#       library set variable LAPACKE_USE_STATIC_LIBS, e.g. with 'cmake -DLAPACKE_USE_STATIC_LIBS=ON ...'.
#       Ref.: https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=902589
find_package(LAPACKE)
set_package_properties(LAPACKE PROPERTIES
    PURPOSE "Optional for matlab adapter."
)

find_package(OpenMP)
set_package_properties(OpenMP PROPERTIES
    PURPOSE "Required for addon elemental."
)

find_package(MPI REQUIRED)
set_package_properties(MPI PROPERTIES
    PURPOSE "Required for distributed computations, e.g. in addon elemental."
    TYPE REQUIRED
)

find_package(Matlab COMPONENTS MAIN_PROGRAM)
set_package_properties(Matlab PROPERTIES
    PURPOSE "Required for addon matlab and unit tests."
)

find_package(FLAME)
set_package_properties(FLAME PROPERTIES
    DESCRIPTION "Accelerated LAPACK algorithms"
    PURPOSE "Required for addon elemental."
)

find_package(Elemental)
set_package_properties(Elemental PROPERTIES
    DESCRIPTION "Distributed linear algebra algorithms"
    PURPOSE "Required for addon elemental."
)

set(QT_USE_IMPORTED_TARGETS TRUE)
find_package(Qt5 COMPONENTS Core Widgets)
set_package_properties(Qt5 PROPERTIES
    PURPOSE "Required for Elemental library."
)

#################### source settings ####################

# put the include dirs which are in the source or build tree before all other include dirs, so the headers in the sources are preferred over the already installed ones
set(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE ON)
# Src: kdelibs/cmake/modules/KDE4Defaults.cmake

set(CXX_TEMPLATE_DEPTH 250            CACHE STRING "Set the maximum instantiation depth for template classes to n (-ftemplate-depth=n).") # increased depth is required by operators.hpp
set(CXX_TEMPLATE_BACKTRACE_LIMIT 250  CACHE STRING "Set the maximum number of template instantiation notes for a single warning or error to n (-ftemplate-backtrace-limit=n).")
set(CXX_ERROR_LIMIT 0                 CACHE STRING "Limits the maximum number of error messages to n (-fmax-errors=n / -ferror-limit=n).")
set(CXX_CONSTEXPR_BACKTRACE_LIMIT 250 CACHE STRING "-fconstexpr-backtrace-limit=n")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# C++14 is required e.g. because of decltype(auto) as return type and constexpr std::forward_as_tuple()!
# C++17 is required for constexpr lambdas, if constexpr(...), constexpr std::array, inline variables, class template argument deduction, std::is_invocable, std::void_t

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# "CMake will generate tests only if the enable_testing() command has been invoked." [1]
# "Note that ctest expects to find a test file in the build directory root. Therefore, this command should be in the 
#  source directory root." [2]
# References:
# [1] https://cmake.org/cmake/help/latest/command/add_test.html#command:add_test
# [2] https://cmake.org/cmake/help/latest/command/enable_testing.html
if(HBRS_MPL_ENABLE_TESTS)
    enable_testing()
endif()

#################### build settings ####################

if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

include(MaybeAddFlags)
maybe_add_c_flag  (HBRS_MPL_HAS_C_WALL                         "-Wall")
maybe_add_cxx_flag(HBRS_MPL_HAS_CXX_WALL                       "-Wall")
#maybe_add_c_flag  (HBRS_MPL_HAS_C_WEXTRA                       "-Wextra")
#maybe_add_cxx_flag(HBRS_MPL_HAS_CXX_WEXTRA                     "-Wextra")
maybe_add_c_flag  (HBRS_MPL_HAS_C_WPEDANTIC                    "-Wpedantic")
maybe_add_cxx_flag(HBRS_MPL_HAS_CXX_WPEDANTIC                  "-Wpedantic")
maybe_add_c_flag  (HBRS_MPL_HAS_C_WERROR_RETURN_TYPE           "-Werror=return-type")
maybe_add_cxx_flag(HBRS_MPL_HAS_CXX_WERROR_RETURN_TYPE         "-Werror=return-type")
maybe_add_c_flag  (HBRS_MPL_HAS_C_PEDANTIC_ERRORS              "-pedantic-errors")
maybe_add_cxx_flag(HBRS_MPL_HAS_CXX_PEDANTIC_ERRORS            "-pedantic-errors")

maybe_add_cxx_flag(HBRS_MPL_HAS_CXX_FTEMPLATE_DEPTH            "-ftemplate-depth=${CXX_TEMPLATE_DEPTH}")
maybe_add_cxx_flag(HBRS_MPL_HAS_CXX_FTEMPLATE_BACKTRACE_LIMIT  "-ftemplate-backtrace-limit=${CXX_TEMPLATE_BACKTRACE_LIMIT}")
maybe_add_cxx_flag(HBRS_MPL_HAS_CXX_FCONSTEXPR_BACKTRACE_LIMIT "-fconstexpr-backtrace-limit=${CXX_CONSTEXPR_BACKTRACE_LIMIT}")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    maybe_add_cxx_flag(HBRS_MPL_HAS_CXX_FERROR_LIMIT           "-ferror-limit=${CXX_ERROR_LIMIT}")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    maybe_add_cxx_flag(HBRS_MPL_HAS_CXX_FMAX_ERRORS            "-fmax-errors=${CXX_ERROR_LIMIT}")
endif()

#maybe_add_cxx_flag(HBRS_MPL_HAS_CXX_AST_DUMP         "-Xclang -ast-dump")

if(NOT HBRS_MPL_HAS_CXX_FTEMPLATE_DEPTH)
    message(FATAL_ERROR "Your C++ compiler ${CMAKE_CXX_COMPILER} does not support '-ftemplate-depth=${CXX_TEMPLATE_DEPTH}', use it at your own risk.")
endif()

if(NOT HBRS_MPL_HAS_CXX_FTEMPLATE_BACKTRACE_LIMIT)
    message(WARNING "Your C++ compiler ${CMAKE_CXX_COMPILER} does not support '-ftemplate-backtrace-limit=${CXX_TEMPLATE_BACKTRACE_LIMIT}', use it at your own risk.")
endif()

if(NOT HBRS_MPL_HAS_CXX_FCONSTEXPR_BACKTRACE_LIMIT)
    message(WARNING "Your C++ compiler ${CMAKE_CXX_COMPILER} does not support '-fconstexpr-backtrace-limit=${CXX_CONSTEXPR_BACKTRACE_LIMIT}', use it at your own risk.")
endif()

#################### install settings ####################

include(GNUInstallDirs)
# Src.: http://www.cmake.org/cmake/help/v2.8.8/cmake.html#module:GNUInstallDirs

# Offer a choice of overriding the installation directories
set(HBRS_MPL_INSTALL_BINDIR      ${CMAKE_INSTALL_BINDIR}                               CACHE PATH "install dir for executables")
set(HBRS_MPL_INSTALL_LIBDIR      ${CMAKE_INSTALL_LIBDIR}                               CACHE PATH "install dir for libraries")
set(HBRS_MPL_INSTALL_INCLUDEDIR  ${CMAKE_INSTALL_INCLUDEDIR}/hbrs/mpl                  CACHE PATH "install dir for headers")
set(HBRS_MPL_INSTALL_CMAKEDIR    ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}   CACHE PATH "install dir for CMake files")

set(HBRS_MPL_INSTALL_TARGETS_DEFAULT_DESTINATIONS
    RUNTIME DESTINATION "${HBRS_MPL_INSTALL_BINDIR}"
    LIBRARY DESTINATION "${HBRS_MPL_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${HBRS_MPL_INSTALL_LIBDIR}"
    PUBLIC_HEADER DESTINATION "${HBRS_MPL_INSTALL_INCLUDEDIR}"
    PRIVATE_HEADER DESTINATION "${HBRS_MPL_INSTALL_INCLUDEDIR}"
    INCLUDES DESTINATION "${HBRS_MPL_INSTALL_INCLUDEDIR}")

#################### list the subdirectories ####################

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(addon)
add_subdirectory(test)
add_subdirectory(benchmark)

#################### install files ####################

include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/hbrs-mpl-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/hbrs-mpl-config.cmake"
    INSTALL_DESTINATION "${HBRS_MPL_INSTALL_CMAKEDIR}"
    PATH_VARS
        HBRS_MPL_INSTALL_BINDIR
        HBRS_MPL_INSTALL_LIBDIR
        HBRS_MPL_INSTALL_INCLUDEDIR
        HBRS_MPL_INSTALL_CMAKEDIR
        HBRS_MPL_ENABLE_ADDON_ELEMENTAL
        HBRS_MPL_ENABLE_ADDON_MATLAB)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/hbrs-mpl-config-version.cmake"
    VERSION ${hbrs-mpl_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/hbrs-mpl-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/hbrs-mpl-config-version.cmake"
    DESTINATION ${HBRS_MPL_INSTALL_CMAKEDIR}
    COMPONENT development)

install(EXPORT hbrs-mpl-targets
  FILE hbrs-mpl-targets.cmake
  NAMESPACE hbrs-mpl::
  DESTINATION ${HBRS_MPL_INSTALL_CMAKEDIR})

#################### summary ####################

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
