set(CMAKE_VERSION 3.20)
cmake_minimum_required(VERSION ${CMAKE_VERSION})

# prevent in-source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source build detected")
endif()

# make sure no libraries from the host platform are used
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

project(function_examples LANGUAGES C CXX ASM)

# set up build for using mlbic instead of other standard lib
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR "${TARGET_CPU}")

# find compiler library (libgcc.a or libclang.a, ...) as that also needs to be linked in
execute_process(COMMAND ${CMAKE_C_COMPILER} -print-file-name=include OUTPUT_VARIABLE NATIVE_INCLUDE OUTPUT_STRIP_TRAILING_WHITESPACE)
cmake_path(SET COMP_INCLUDE NORMALIZE ${NATIVE_INCLUDE})
execute_process(COMMAND ${CMAKE_C_COMPILER} -print-libgcc-file-name OUTPUT_VARIABLE NATIVE_RTLIB OUTPUT_STRIP_TRAILING_WHITESPACE)
cmake_path(SET COMP_RTLIB NORMALIZE ${NATIVE_RTLIB})
foreach(LANG C CXX)
  set(CMAKE_${LANG}_STANDARD_INCLUDE_DIRECTORIES ${COMP_INCLUDE})
endforeach()

add_library(compiler_lib SHARED IMPORTED)
set_target_properties(compiler_lib PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${COMP_INCLUDE})
set_target_properties(compiler_lib PROPERTIES IMPORTED_LOCATION "${COMP_RTLIB}")
target_compile_options(compiler_lib INTERFACE -nostdinc)
target_link_options(compiler_lib INTERFACE -nostdlib -Wl,--image-base=0)

# prepare mlibc 
# make cross file
configure_file(cross_template.txt crossfile.txt)
include(ExternalProject)
ExternalProject_Add(mlibc
SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../mlibc
CONFIGURE_COMMAND meson setup --cross-file ${CMAKE_CURRENT_BINARY_DIR}/crossfile.txt -Dprefix=<INSTALL_DIR> <BINARY_DIR> <SOURCE_DIR>
BUILD_COMMAND ninja -C <BINARY_DIR>
BUILD_ALWAYS TRUE
INSTALL_COMMAND meson install --no-rebuild --only-changed -C <BINARY_DIR>
)
ExternalProject_Get_Property(mlibc INSTALL_DIR)

add_library(dlibc OBJECT IMPORTED)
add_dependencies(dlibc mlibc)
set(MLIBC_OBJ 
  "${INSTALL_DIR}/lib/crt1.o"
  "${INSTALL_DIR}/lib/crti.o"
  "${INSTALL_DIR}/lib/crtn.o")
set_target_properties(dlibc PROPERTIES IMPORTED_OBJECTS "${MLIBC_OBJ}")
file(MAKE_DIRECTORY "${INSTALL_DIR}/include")
target_include_directories(dlibc INTERFACE "${INSTALL_DIR}/include")
set(MLIBC_LIBS
  "${INSTALL_DIR}/lib/libc.a"
  "${INSTALL_DIR}/lib/libcrypt.a"
  "${INSTALL_DIR}/lib/libdl.a"
  "${INSTALL_DIR}/lib/libm.a"
  "${INSTALL_DIR}/lib/libpthread.a"
  "${INSTALL_DIR}/lib/libresolv.a"
  "${INSTALL_DIR}/lib/librt.a"
  "${INSTALL_DIR}/lib/libutil.a"
)
target_link_libraries(dlibc INTERFACE compiler_lib ${MLIBC_LIBS})
target_compile_options(dlibc INTERFACE -nostdinc)
target_link_options(dlibc INTERFACE -nostdlib)

# cheri compile flags
if("${DANDELION_PLATFORM}" STREQUAL "cheri")
set(NOCAP_COMPILE_FLAGS -march=morello -mabi=aapcs)
set(NOCAP_LINK_FLAGS -march=morello -mabi=aapcs -fuse-ld=lld)
add_compile_options(${NOCAP_COMPILE_FLAGS})
endif()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../dandelionFunctionInterface" functionInterface)

# add folders with function examples
# add_subdirectory(access)
add_subdirectory(basic)
add_subdirectory(matmac)
add_subdirectory(matmul)
add_subdirectory(stdio)
add_subdirectory(files)