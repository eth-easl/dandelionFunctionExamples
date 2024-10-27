set(CMAKE_VERSION 3.23)
cmake_minimum_required(VERSION ${CMAKE_VERSION})

project(function_examples LANGUAGES C CXX ASM)

include(FetchContent)

# option to include libc-dependent examples
set(ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR} CACHE STRING "the architecture to build for")
set(PLATFORM "debug" CACHE STRING "backend to build for")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

message(STATUS "Building ${CMAKE_BUILD_TYPE} for ${PLATFORM} on ${ARCHITECTURE}")

# prevent in-source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source build detected")
endif()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "${CMAKE_BINARY_DIR}/dandelion_sdk")
endif()

# setup build toolchain
FetchContent_Declare(
  dandelion_sdk
  DOWNLOAD_COMMAND gh release download latest -R eth-easl/dandelionFunctionInterface -p "*${CMAKE_BUILD_TYPE}_${PLATFORM}_${ARCHITECTURE}.tar.gz"
  COMMAND tar -xzf "dandelion_sdk_${CMAKE_BUILD_TYPE}_${PLATFORM}_${ARCHITECTURE}.tar.gz" -C ${CMAKE_BINARY_DIR}
)
FetchContent_MakeAvailable(dandelion_sdk)

execute_process(
    COMMAND ${CMAKE_C_COMPILER} -print-file-name=include
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE COMPILER_RUNTIME_INCLUDE
    COMMAND_ERROR_IS_FATAL ANY
)

add_library(dandelion_runtime INTERFACE IMPORTED)
target_include_directories(dandelion_runtime INTERFACE
  "${CMAKE_BINARY_DIR}/dandelion_sdk/include"
)
target_compile_options(dandelion_runtime INTERFACE -nostdinc -idirafter${COMPILER_RUNTIME_INCLUDE})
target_link_libraries(dandelion_runtime INTERFACE
  "${CMAKE_BINARY_DIR}/dandelion_sdk/lib/libdandelion_runtime.a"
  "${CMAKE_BINARY_DIR}/dandelion_sdk/lib/libdandelion_system.a"
)
target_link_options(dandelion_runtime INTERFACE -static -nostdlib)

add_library(dlibc INTERFACE IMPORTED)
target_include_directories(dlibc INTERFACE
  "${CMAKE_BINARY_DIR}/dandelion_sdk/include"
  "${CMAKE_BINARY_DIR}/dandelion_sdk/include/sys"
)
target_link_libraries(dlibc INTERFACE
  "${CMAKE_BINARY_DIR}/dandelion_sdk/lib/libc.a"
  "${CMAKE_BINARY_DIR}/dandelion_sdk/lib/libg.a"
  "${CMAKE_BINARY_DIR}/dandelion_sdk/lib/libm.a"
  "${CMAKE_BINARY_DIR}/dandelion_sdk/lib/libdandelion_file_system.a"
)
target_link_options(dlibc INTERFACE -T${CMAKE_BINARY_DIR}/dandelion_sdk/linker.ld)

add_library(dlibcxx INTERFACE IMPORTED)
target_include_directories(dlibcxx INTERFACE "${CMAKE_BINARY_DIR}/dandelion_sdk/include/c++/v1")
target_link_libraries(dlibcxx INTERFACE
  "${CMAKE_BINARY_DIR}/dandelion_sdk/lib/libc++.a"
  "${CMAKE_BINARY_DIR}/dandelion_sdk/lib/libc++abi.a"
  "${CMAKE_BINARY_DIR}/dandelion_sdk/lib/libunwind.a"
)

# create compiler wrapper for use to compile non integrated programs
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cfg_template.txt
  ${CMAKE_BINARY_DIR}/dandelion.cfg
  FILE_PERMISSIONS OWNER_READ OWNER_EXECUTE
)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    add_compile_options(-gdwarf-4)
endif()

# cheri compile flags
if("${DANDELION_PLATFORM}" STREQUAL "cheri")
  add_compile_options(-march=morello -mabi=aapcs)
  add_link_options(-march=morello -mabi=aapcs -fuse-ld=lld -Xlinker --image-base=0x10000)
  get_property(CROSS_COMPILE_OPTIONS DIRECTORY PROPERTY COMPILE_OPTIONS)
  string(REPLACE ";" " " CROSS_COMPILE_OPTIONS "${CROSS_COMPILE_OPTIONS}")
  get_property(CROSS_LINK_OPTIONS DIRECTORY PROPERTY LINK_OPTIONS)
  string(REPLACE ";" " " CROSS_LINK_OPTIONS "${CROSS_LINK_OPTIONS}")
# WebAssembly flags
elseif("${DANDELION_PLATFORM}" STREQUAL "wasm")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --verbose -target wasm32-unknown-unknown -Wl,--export-all -Wl,--import-undefined -Wl,--import-memory")
else()
# currently still want to set the base image lower for all other images, if some don't want make special case
  add_link_options(-fuse-ld=lld -Xlinker --image-base=0x10000)
endif()

# add folders with function examples
# add_subdirectory(access)
add_subdirectory(basic)
add_subdirectory(busy)
add_subdirectory(matmac)
add_subdirectory(matmul)
add_subdirectory(example_app_nolibc)

# dependent on dlibc
add_subdirectory(dirigent_busy)
add_subdirectory(example_app)
add_subdirectory(files)
add_subdirectory(stdio)
add_subdirectory(stdio_cxx)
add_subdirectory(example_app_hybrid)
