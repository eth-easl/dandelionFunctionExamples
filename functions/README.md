# Functions

This folder contains a series of functions used to test basic functionality.
The functions are split into multiple executables that can be built individually.
Some functions depend on the function interface directly while others use it 
through mlibc.

# Building the C functions
The cmake build process will build the function interface, libc and the functions.
To do so cmake needs to know that it needs to use the cross compile file dandelion.
Additionally, cmake and needs to know the platform to build for (cheri, mmu_freebsd, mmu_linux).
From the top folder functions can be built with:
```
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../functions/dandelion.cmake -DDANDELION_PLATFORM=<platform> -DTARGET_CPU=<target arch> ../functions
```
The mlibc build currently only seems to work with clang.
To set this up set CC=clang and CXX=clang++ additionally set LDFLAGS=-fuse-ld=lld

## Building dlibc
To turn on building of libc enable the cmake option with `-DUSE_LIBC=on`