cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project("tlocpp")
enable_testing()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(flags "-std=c++14 -pedantic -Wall -Wextra -Werror")
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(flags "${flags} -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-exit-time-destructors -Wno-global-constructors")
  endif()
  set(debug_flags "-g")
  #set(debug_flags "-g -fsanitize=address -fsanitize=undefined -O1 -fno-omit-frame-pointer -fno-optimize-sibling-calls")
  #set(debug_flags "-g -fsanitize=memory -fsanitize=undefined -O1 -fno-omit-frame-pointer -fno-optimize-sibling-calls")
else()
  set(flags "${CMAKE_CXX_FLAGS}")
  set(debug_flags "${CMAKE_CXX_FLAGS_DEBUG}")
endif()

message(STATUS "tlocpp setting flags")
set(CMAKE_CXX_FLAGS "${flags}" CACHE STRING
    "Flags used by the compiler during all build types." FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "${debug_flags}" CACHE STRING
    "Flags used by the compiler during debug builds." FORCE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(tlo)
add_executable(tlocpp_test darray_test.cpp ctci_test.cpp bigint_test.cpp test_test.cpp)
target_link_libraries(tlocpp_test tlocpp tlocpptestmain)
add_test(tlocpp_test tlocpp_test)
