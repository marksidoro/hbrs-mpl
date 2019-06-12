# Copyright (c) 2018 Jakob Meng, <jakobmeng@web.de>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

if (HBRS_MPL_ENABLE_ELEMENTAL)
    add_subdirectory(elemental)
endif()

if (HBRS_MPL_ENABLE_MATLAB)
    add_subdirectory(matlab)
endif()
