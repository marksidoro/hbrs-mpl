# Copyright (c) 2016-2018 Jakob Meng, <jakobmeng@web.de>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

############## benchmarks ##############

add_custom_target(benchmarks COMMENT "Run all the benchmarks.")

#################### list the subdirectories ####################

add_subdirectory(fn)

