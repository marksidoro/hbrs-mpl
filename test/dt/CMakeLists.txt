# Copyright (c) 2016 Jakob Meng, <jakobmeng@web.de>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

############## tests ##############

get_filename_component(_pwd "${CMAKE_CURRENT_LIST_DIR}" NAME)
add_custom_target(_${_pwd}_tests)

foreach(target ctsam ctsav escsv functor range rtsam rtsav scv sm sms srv submatrix subsequence uscsv)
	hbrs_mpl_add_test(${target})
	target_link_libraries(${target} fuse_lib)
	add_dependencies(_${_pwd}_tests ${target})
endforeach()