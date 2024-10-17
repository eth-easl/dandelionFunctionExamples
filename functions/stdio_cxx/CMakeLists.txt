cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(EXECUTABLE "stdio_cxx")

add_executable(${EXECUTABLE}
  stdio.cpp
)

target_compile_options(${EXECUTABLE} PRIVATE
  -gdwarf-4
  -static
  -O0
  -nostdinc
  -I/mnt/c/Users/kuchlert/Documents/fuseArch/dandelionProj/dandelionFunctionExamples/build_debug/dandelion_sdk/include/c++/v1
  # -I/mnt/c/Users/kuchlert/Documents/fuseArch/dandelionProj/dandelionFunctionExamples/build_debug/dandelion_sdk/include/machine
  # -I/mnt/c/Users/kuchlert/Documents/fuseArch/dandelionProj/dandelionFunctionExamples/build_debug/dandelion_sdk/include/ssp
  -I/mnt/c/Users/kuchlert/Documents/fuseArch/dandelionProj/dandelionFunctionExamples/build_debug/dandelion_sdk/include
  -idirafter/mnt/c/Users/kuchlert/Documents/fuseArch/dandelionProj/dandelionFunctionExamples/build_debug/dandelion_sdk/include/sys
  -I/usr/lib/llvm-14/lib/clang/14.0.6/include
)
target_link_options(${EXECUTABLE} PRIVATE -static
  -nostdlib
  -L/mnt/c/Users/kuchlert/Documents/fuseArch/dandelionProj/dandelionFunctionExamples/build_debug/dandelion_sdk/lib
  -lc
  -lc++
  -lc++abi
  -lg
  -lm
  -ldandelion_file_system
  -ldandelion_runtime
  -ldandelion_system
  -lunwind
)
target_link_libraries(${EXECUTABLE} PRIVATE
  dlibcxx
)