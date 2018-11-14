cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project("tlocpp")
enable_testing()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(warning_compile_options -pedantic -Wall -Wextra -Werror)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warning_compile_options ${warning_compile_options} -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic)
  endif()
endif()

add_subdirectory(src)
add_subdirectory(test)
