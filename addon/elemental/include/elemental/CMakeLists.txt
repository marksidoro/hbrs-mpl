# Copyright (c) 2018 Jakob Meng, <jakobmeng@web.de>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

#################### fuse ####################

target_fuse_tags(fuse_lib "ELEMENTAL_FUSE")
target_fuse_directories(fuse_lib "${CMAKE_CURRENT_SOURCE_DIR}/fuse/" INCLUDE_PREFIX "elemental/fuse")

#################### install files ####################

install(FILES
    config.hpp
    DESTINATION "${HBRS_MPL_INSTALL_ELEMENTAL_INCLUDEDIR}"
    COMPONENT development)

install(DIRECTORY ./
    DESTINATION "${HBRS_MPL_INSTALL_ELEMENTAL_INCLUDEDIR}"
    COMPONENT development
    FILES_MATCHING PATTERN "*.hpp")
