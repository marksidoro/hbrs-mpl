# Copyright (c) 2016-2018 Jakob Meng, <jakobmeng@web.de>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

if(HBRS_MPL_ENABLE_MEMCHECK AND NOT VALGRIND)
	message(FATAL_ERROR "Valgrind not found. Required dependency of tests.")
endif()

############## tests ##############

# CMake's target test does not build but only executes tests
# Ref.: https://stackoverflow.com/a/736838/6490710
if(HBRS_MPL_ENABLE_TESTS)
	set(_MAYBE_ALL ALL)
endif()
add_custom_target(tests ${_MAYBE_ALL} COMMENT "Build all tests.")

function(hbrs_mpl_add_test target)
	add_executable(${target} EXCLUDE_FROM_ALL "${target}.cpp")
	target_link_libraries(${target} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
	target_include_directories(${target}
		SYSTEM
		PUBLIC ${Boost_INCLUDE_DIRS})
	
	if(HBRS_MPL_ENABLE_MEMCHECK AND VALGRIND)
		add_test(${target} ${VALGRIND} --leak-check=full --error-exitcode=1 ${CMAKE_CURRENT_BINARY_DIR}/${target})
	else()
		add_test(${target} ${CMAKE_CURRENT_BINARY_DIR}/${target})
	endif()
	
	add_dependencies(tests ${target})
	
	if(HBRS_MPL_ENABLE_OBJDUMP)
		hbrs_mpl_disassemble(${target})
	endif()
endfunction()

get_filename_component(_pwd "${CMAKE_CURRENT_LIST_DIR}" NAME)
add_custom_target(_${_pwd}_tests)

foreach(target)
	hbrs_mpl_add_test(${target})
	add_dependencies(_${_pwd}_tests ${target})
endforeach()

#################### list the subdirectories ####################

add_subdirectory(detail)
add_subdirectory(core)
add_subdirectory(dt)
add_subdirectory(fn)
add_subdirectory(addon)
