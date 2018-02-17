macro(prepend listVar prefix)
   set(${listVar} "")
   foreach(item ${ARGN})
      list(APPEND ${listVar} ${prefix}${item})
   endforeach(item)
endmacro(prepend)

add_library(tlocpp_headers_only INTERFACE)
target_include_directories(tlocpp_headers_only INTERFACE ${PROJECT_SOURCE_DIR}/include)

set(tlocpp_headers util.hpp darray.hpp ctci.hpp bigint.hpp test.hpp)
set(tlocpp_sources util.cpp ctci.cpp bigint.cpp test.cpp)
prepend(tlocpp_headers ${PROJECT_SOURCE_DIR}/include/tlo/ ${tlocpp_headers})
add_library(tlocpp STATIC ${tlocpp_headers} ${tlocpp_sources})
target_include_directories(tlocpp PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(tlocpp_testmain STATIC testmain.cpp)
target_include_directories(tlocpp_testmain PUBLIC ${PROJECT_SOURCE_DIR}/include)
