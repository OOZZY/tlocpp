# Clone, Build, and Test

```
$ git clone <url/to/tlocpp.git> # clone into tlocpp directory
$ mkdir tlocppbuild
$ cd tlocppbuild
$ cmake -G 'Unix Makefiles' -DCMAKE_BUILD_TYPE=Debug ../tlocpp
$ make
$ make test # run tests with make
$ ./tlocpp_test # run tests directly
```
