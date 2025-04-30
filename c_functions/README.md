# Functions

This folder contains a series of functions used for basic functionality tests or as example applications. 
The functions are split into multiple executables that can be built individually.

All C and C++ functions depend on the [dandelion SDK](https://github.com/eth-easl/dandelion).
Functions can either only use the lowest level interface `dandelion_runtime` or the `dlibc`/`dlibcxx`.
The runtime provides basic wrapper to start a function and interact directly with input and output sets and buffers.
The libc variants also support reading and writing inputs and outputs to a filesystem abstraction, which is further explained in the SDK documentation.

# Building

The build for this repository is set up to allow building functions for all backends and architectures supported by the SDK.
To set this up use the following commands:

```
mkdir build
cd build
cmake -DPLATFORM=<platform> -DARCHITECTURE=<target arch> -DCMAKE_BUILD_TYPE-<type> .
```

The platforms supported are `CHERI`, `MMU_FREEBSD`, `MMU_LINUX`, `KVM`, `WASM`, `DEBUG`. \
We support `x86_64` and `aarch64` architectures. \
The build type can be either `Debug` or `Release`.

When one of the parameters is not given, it defaults to the `DEBUG` platform, the build system architecture and `Debug` build type.

The CMake in this project is set up to automatically download the latest version of the dandelion SDK when it is initially set up.
The easiest way to use a different version, is to replace the `dandelion_sdk` folder in the build folder with a symlink to the version to be used.