# Copyright (c) 2019 Jakob Meng, <jakobmeng@web.de>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

#################### tests ####################

if (HBRS_MPL_ENABLE_ELEMENTAL)
    hbrs_mpl_add_test(detail_operators_elemental "elemental.cpp")
endif()

hbrs_mpl_add_test(detail_operators_hbrs_mpl "hbrs_mpl.cpp")

if (HBRS_MPL_ENABLE_MATLAB)
    hbrs_mpl_add_test(detail_operators_matlab "matlab.cpp")
endif()
