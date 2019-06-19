# Copyright (c) 2019 Jakob Meng, <jakobmeng@web.de>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

#################### benchmarks ####################

foreach(name
    std)
    hbrs_mpl_add_benchmark(benchmark_fn_at_${name} "${name}.cpp")
endforeach()

