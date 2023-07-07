set(CMAKE_VERSION 3.13)
cmake_minimum_required(VERSION ${CMAKE_VERSION})

# prevent in-source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source build detected")
endif()

# set up build for using mlbic instead of other standard lib
set(CMAKE_SYSTEM_NAME Generic)

# make sure no libraries from the host platform are used
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

project(function_examples LANGUAGES C CXX ASM)

# find compiler library (libgcc.a or libclang.a, ...) as that also needs to be linked in
find_library(COMPILER_LIB gcc clang REQUIRED PATHS ${CMAKE_C_COMPILER}/../../lib/)

# prepare mlibc 
# make cross file
configure_file(cross_template.txt crossfile.txt)
include(ExternalProject)
ExternalProject_Add(mlibc
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../mlibc
    # GIT_REPOSITORY "git@github.com:eth-easl/mlibc.git"
    # GIT_TAG "origin/dandelion"
    # CONFIGURE_COMMAND echo skipping
    CONFIGURE_COMMAND meson setup --cross-file ${CMAKE_CURRENT_BINARY_DIR}/crossfile.txt -Dprefix=<INSTALL_DIR> <BINARY_DIR> <SOURCE_DIR>
    BUILD_COMMAND ninja -C <BINARY_DIR>
    BUILD_ALWAYS TRUE
    INSTALL_COMMAND meson install -C <BINARY_DIR>
)
ExternalProject_Get_Property(mlibc INSTALL_DIR)
add_compile_options(-nostdinc -isystem ${INSTALL_DIR}/include})
add_link_options(-nostdlib -rphat ${MLIBC_INCLUDE_DIR}/lib)

# cheri compile flags
set(NOCAP_COMPILE_FLAGS -march=morello -mabi=aapcs)
set(NOCAP_LINK_FLAGS -march=morello -mabi=aapcs -fuse-ld=lld)
add_compile_options(${NOCAP_COMPILE_FLAGS})

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../dandelionFunctionInterface" functionInterface)

# add folders with function examples
# add_subdirectory(access)
add_subdirectory(basic)
add_subdirectory(matmul)
add_subdirectory(stdio)
add_subdirectory(files)