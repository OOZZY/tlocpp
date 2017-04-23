# tlocpp

A C++ library.

## Build Requirements

* CMake
* C++14 development environment for which CMake can generate build files

## Clone, Build, and Test

```
$ git clone --branch develop <url/to/tlocpp.git> # clone into tlocpp directory
$ mkdir tlocppbuild
$ cd tlocppbuild
$ cmake -G 'Unix Makefiles' -DCMAKE_BUILD_TYPE=Debug ../tlocpp
$ make
$ make test # run tests with make
$ ./tlocpp_test # run tests directly
```
