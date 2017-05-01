add_library(tlocpp STATIC util.cpp ctci.cpp bigint.cpp test.cpp)
add_library(tlocpptestmain STATIC testmain.cpp)

# make headers of header-only modules show up in IDE project
add_custom_target(tlocpptloheaders SOURCES darray.hpp)
