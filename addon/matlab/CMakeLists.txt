# Copyright (c) 2016-2018 Jakob Meng, <jakobmeng@web.de>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

if (NOT MATLAB_FOUND)
	message(FATAL_ERROR "MATLAB not found. Required dependency of addon matlab.")
endif()

#################### install settings ####################

include(GNUInstallDirs)
set(HBRS_MPL_INSTALL_MATLAB_INCLUDEDIR      ${HBRS_MPL_INSTALL_INCLUDEDIR}/addon/matlab     CACHE PATH "install dir for addon matlab headers")
set(HBRS_MPL_INSTALL_MATLAB_CXN_INCLUDEDIR  ${HBRS_MPL_INSTALL_INCLUDEDIR}/addon/matlab/cxn CACHE PATH "install dir for addon matlab cxn headers")

#################### list the subdirectories ####################

add_subdirectory(cxn)
add_subdirectory(include)
add_subdirectory(src)

