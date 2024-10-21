# Functions

This folder contains a series of functions used to test basic functionality.
The functions are split into multiple executables that can be built individually.
Some functions depend on the function interface directly while others use it 
through mlibc.

# Building the C functions
The cmake build process will build the functions based on the latest version of the dandelion SDK.
For this cmake meeds to know the platform to build for (cheri, mmu_freebsd, mmu_linux, debug) and architecture.
From the top folder functions can be built with:
```
mkdir build
cd build
cmake -DPLATFORM=<platform> -DARCHITECTURE=<target arch> ../functions
```
