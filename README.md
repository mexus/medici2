medici2
=======

A library, cli (in plans) and gui (in plans) for calculations on a "Medici patience".


Warning!
========

The project is under a heavy development, so anything could happen!


Requirements
============

1. CMake to build tests.
2. Some C++11 functionality used (shared pointers, c++11 initializations, auto keyword, ...), so C++11 compliant compiler required.

Tested with:
1. clang++ 3.4.2 AND g++ 4.9.1, cmake 3.0.0, linux kernel 3.15.7-1-ck (x86_64).


Building
========

Go to a "build" folder and execute:

% cmake ..

% make

Consider running tests via ./test/medici-test and ./performance/performance-test.


Using the library
=================

See usage at test/ folder.


Code analysis
====================

Static code analysis is done on a regular basis by means of clang (scan-build) and cppcheck.

Dynamic code analisis is done on a regular basis by valgrind.


