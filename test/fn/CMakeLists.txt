# Copyright (c) 2016-2018 Jakob Meng, <jakobmeng@web.de>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

############## tests ##############

get_filename_component(_pwd "${CMAKE_CURRENT_LIST_DIR}" NAME)
add_custom_target(_${_pwd}_tests)

foreach(target append apply apply_at at columns contains expand first fold1_left fold_left if insert invoke iterate mean operators pca pca_filter recurse second signum size svd transform where zip)
	hbrs_mpl_add_test(${target})
	add_dependencies(_${_pwd}_tests ${target})
	target_link_libraries(${target} fuse_lib)
endforeach()

unset(maybe_tests)
