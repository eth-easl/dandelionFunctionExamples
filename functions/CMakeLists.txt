set(CMAKE_VERSION 3.13)
cmake_minimum_required(VERSION ${CMAKE_VERSION})

# prevent in-source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source build detected")
endif()

# make sure no libraries from the host platform are used
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

project(function_examples LANGUAGES C CXX ASM)

# set up build for using mlbic instead of other standard lib
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR "${TARGET_CPU}")

# find compiler library (libgcc.a or libclang.a, ...) as that also needs to be linked in
# execute_process(COMMAND ${CMAKE_C_COMPILER} -print-libgcc-file-name OUTPUT_VARIABLE NATIVE_PATH)
# cmake_path(SET CMAKE_COMP_LIB_PATH "${NATIVE_PATH}")
# cmake_path(GET CMAKE_COMP_LIB_PATH PARENT_PATH LIB_FOLDER)
# find_library(COMPILER_LIB NAMES gcc clang REQUIRED HINTS "${LIB_FOLDER}")
execute_process(COMMAND ${CMAKE_C_COMPILER} -print-file-name=include OUTPUT_VARIABLE NATIVE_INCLUDE)
cmake_path(SET COMP_INCLUDE "${NATIVE_INCLUDE}")

# prepare mlibc 
# make cross file
configure_file(cross_template.txt crossfile.txt)
include(ExternalProject)
ExternalProject_Add(mlibc
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../mlibc
    CONFIGURE_COMMAND meson setup --cross-file ${CMAKE_CURRENT_BINARY_DIR}/crossfile.txt -Dprefix=<INSTALL_DIR> <BINARY_DIR> <SOURCE_DIR>
    BUILD_COMMAND ninja -C <BINARY_DIR>
    BUILD_ALWAYS TRUE
    INSTALL_COMMAND meson install -C <BINARY_DIR>
)
ExternalProject_Get_Property(mlibc INSTALL_DIR)
include_directories(SYSTEM "${INSTALL_DIR}/include")
# include_directories("${COMP_INCLUDE}")
foreach(LANG C CXX)
    set(CMAKE_${LANG}_STANDARD_INCLUDE_DIRECTORIES ${COMP_INCLUDE})
    # set(CMAKE_${LANG}_STANDARD_LIBRARIES ${MLIBC} ) 
endforeach()
add_compile_options(-nostdinc) 
add_link_options(-nostdlib -rpath ${MLIBC_INCLUDE_DIR}/lib)

# cheri compile flags
if(${DANDELION_PLATFORM} STREQUAL "cheri")
set(NOCAP_COMPILE_FLAGS -march=morello -mabi=aapcs)
set(NOCAP_LINK_FLAGS -march=morello -mabi=aapcs -fuse-ld=lld)
add_compile_options(${NOCAP_COMPILE_FLAGS})
endif()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../dandelionFunctionInterface" functionInterface)

# add folders with function examples
# add_subdirectory(access)
add_subdirectory(basic)
add_subdirectory(matmul)
add_subdirectory(stdio)
add_subdirectory(files)