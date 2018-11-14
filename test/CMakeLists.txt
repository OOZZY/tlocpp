set(tlocpp_test_headers tlocpp_test.hpp)
set(tlocpp_test_sources darray_test.cpp ctci_test.cpp bigint_test.cpp test_test.cpp)
add_executable(tlocpp_test ${tlocpp_test_headers} ${tlocpp_test_sources})
set_target_properties(tlocpp_test PROPERTIES CXX_EXTENSIONS OFF)
target_compile_options(tlocpp_test PRIVATE ${warning_compile_options})
target_include_directories(tlocpp_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tlocpp_test PRIVATE tlocpp_headers_only tlocpp tlocpp_testmain)

add_test(NAME tlocpp_test COMMAND tlocpp_test)
