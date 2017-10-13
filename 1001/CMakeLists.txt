cmake_minimum_required(VERSION 2.8)

project(poj_1001)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")
    set(common_base_flags "${common_base_flags} -O0 -g")
else()
    set(common_base_flags "${common_base_flags} -O3")
endif()

set(common_base_flags "${common_base_flags} -std=gnu99")

add_executable(poj_1001)
set_target_properties(poj_1001
    PROPERTIES
    COMPILE_FLAGS "${common_base_flags}")

