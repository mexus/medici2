medici2
=======

A library, cli (in plans) and gui (in plans) for calculations on a "Medici patience".


Warning!
========

The project is under a heavy development, so anything could happen!


Requirements
============

1. CMake to build tests.
2. C++11 compliant compiler required.
3. [Easylogging++](https://github.com/easylogging/easyloggingpp) header-only library.

Tested with:
1. clang++ 3.6.2 AND g++ 5.2.0, cmake 3.3.0.


Building
========

Go to a [build](/build/) folder and execute:

```
% cmake ..
% make
```

Consider running tests via `./test/medici-test` and `./performance/performance-test`.


Using the library
=================

See usage at [test](/test/) folder.


Code analysis
====================

Static code analysis is done on a regular basis by means of clang (`scan-build`) and cppcheck.

Dynamic code analisis is done on a regular basis by valgrind and clang `-fsanitize=address`.
