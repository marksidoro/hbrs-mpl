# Copyright (c) 2018 Jakob Meng, <jakobmeng@web.de>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

if (NOT OpenMP_FOUND)
	message(FATAL_ERROR "OpenMP not found. Required dependency of addon elemental.")
endif()

if (NOT MPI_CXX_FOUND)
	message(FATAL_ERROR "MPI C API for C++ not found. Required dependency of addon elemental.")
endif()

if (NOT FLAME_FOUND)
	message(FATAL_ERROR "FLAME library not found. Required dependency of addon elemental.")
endif()

if (NOT TARGET El)
	message(FATAL_ERROR "Elemental library not found. Required dependency of addon elemental.")
endif()

#################### install settings ####################

include(GNUInstallDirs)
set(HBRS_MPL_INSTALL_ELEMENTAL_INCLUDEDIR  ${HBRS_MPL_INSTALL_INCLUDEDIR}/addon/elemental CACHE PATH "install dir for addon elemental headers")

#################### list the subdirectories ####################

add_subdirectory(include)
add_subdirectory(src)
