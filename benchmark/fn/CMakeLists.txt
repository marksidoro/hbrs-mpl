# Copyright (c) 2016-2018 Jakob Meng, <jakobmeng@web.de>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

include(DevTools)

foreach(app at)
	add_executable(benchmark_${app} EXCLUDE_FROM_ALL ${app}.cpp)
	target_link_libraries(benchmark_${app} fuse_lib)
	maybe_disassemble(benchmark_${app})
	maybe_callgrind(benchmark_${app})
	maybe_gperftools_profile(benchmark_${app})
	maybe_perf(benchmark_${app})
	add_dependencies(benchmarks benchmark_${app})
endforeach()
