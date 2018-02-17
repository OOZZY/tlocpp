# tlocpp

A C++ library.

## Build Requirements

* CMake
* C++14 development environment for which CMake can generate build files

## Clone, Build, and Test

Clone into tlocpp directory.

```
$ git clone --branch develop <url/to/tlocpp.git>
```

Build.

```
$ mkdir tlocppbuild
$ cd tlocppbuild
$ cmake -G 'Unix Makefiles' -DCMAKE_BUILD_TYPE=Debug ../tlocpp
$ make
```

Run tests with make.

```
$ make test
```

Run tests directly.

```
$ ./test/tlocpp_test
```
