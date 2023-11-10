set(CMAKE_VERSION 3.20)
cmake_minimum_required(VERSION ${CMAKE_VERSION})

# prevent in-source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source build detected")
endif()

# we are cross-compiling; prevents:
# https://stackoverflow.com/questions/53633705/cmake-the-c-compiler-is-not-able-to-compile-a-simple-test-program
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

project(function_examples LANGUAGES C CXX ASM)

if("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
else()
  message(FATAL_ERROR "Compiler not clang\n" "${CMAKE_C_COMPILER_ID}")
endif()

# find compiler library (libgcc.a or libclang.a, ...) as that also needs to be linked in
# execute_process(COMMAND ${CMAKE_C_COMPILER} -print-file-name=include OUTPUT_VARIABLE NATIVE_INCLUDE OUTPUT_STRIP_TRAILING_WHITESPACE)
# cmake_path(SET COMP_INCLUDE NORMALIZE ${NATIVE_INCLUDE})
# foreach(LANG C CXX)
# set(CMAKE_${LANG}_STANDARD_INCLUDE_DIRECTORIES ${COMP_INCLUDE})
# endforeach()

# add_library(compiler_lib STATIC IMPORTED)
# set_target_properties(compiler_lib PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${COMP_INCLUDE})
# set_target_properties(compiler_lib PROPERTIES IMPORTED_LOCATION "${COMP_RTLIB}")
# target_compile_options(compiler_lib INTERFACE -nostdinc)
# target_link_options(compiler_lib INTERFACE -nostdlib -Wl)
# target_link_options(compiler_lib INTERFACE -nostdlib -Wl --image-base=0)

# # prepare mlibc 
# # make cross file
# configure_file(cross_template.txt crossfile.txt)
# include(ExternalProject)
# ExternalProject_Add(mlibc
# SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../mlibc
# CONFIGURE_COMMAND meson setup --cross-file ${CMAKE_CURRENT_BINARY_DIR}/crossfile.txt -Dprefix=<INSTALL_DIR> <BINARY_DIR> <SOURCE_DIR>
# BUILD_COMMAND ninja -C <BINARY_DIR>
# # Enable when working on the libc to make sure it is freshly compiled
# # BUILD_ALWAYS TRUE
# INSTALL_COMMAND meson install -C <BINARY_DIR> --no-rebuild --only-changed --quiet
# )
# ExternalProject_Get_Property(mlibc INSTALL_DIR)
# set(MLIBC_INSTALL_DIR ${INSTALL_DIR})

# # mlibc can depend on compiler runtime library so that also needs to be linked
# execute_process(COMMAND ${CMAKE_C_COMPILER} -print-libgcc-file-name OUTPUT_VARIABLE NATIVE_RTLIB OUTPUT_STRIP_TRAILING_WHITESPACE)
# cmake_path(SET COMP_RTLIB NORMALIZE ${NATIVE_RTLIB})

# add_library(dlibc OBJECT IMPORTED)
# add_dependencies(dlibc mlibc)
# set(MLIBC_OBJ 
#   "${MLIBC_INSTALL_DIR}/lib/crt1.o"
#   "${MLIBC_INSTALL_DIR}/lib/crti.o"
#   "${MLIBC_INSTALL_DIR}/lib/crtn.o")
# set_target_properties(dlibc PROPERTIES IMPORTED_OBJECTS "${MLIBC_OBJ}")
# file(MAKE_DIRECTORY "${MLIBC_INSTALL_DIR}/include")
# target_include_directories(dlibc INTERFACE "${MLIBC_INSTALL_DIR}/include")
# set(MLIBC_LIBS
#   "${MLIBC_INSTALL_DIR}/lib/libc.a"
#   "${MLIBC_INSTALL_DIR}/lib/libcrypt.a"
#   "${MLIBC_INSTALL_DIR}/lib/libdl.a"
#   "${MLIBC_INSTALL_DIR}/lib/libm.a"
#   "${MLIBC_INSTALL_DIR}/lib/libpthread.a"
#   "${MLIBC_INSTALL_DIR}/lib/libresolv.a"
#   "${MLIBC_INSTALL_DIR}/lib/librt.a"
#   "${MLIBC_INSTALL_DIR}/lib/libutil.a"
#   ${COMP_RTLIB}
# )
# target_link_libraries(dlibc INTERFACE ${RUNTIME_LIB} ${MLIBC_LIBS})
# target_link_options(dlibc INTERFACE -nostdlib)

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
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --verbose -target wasm32-unknown-unknown -Wl,--export-all -Wl,--import-undefined")
endif()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../dandelionFunctionInterface" functionInterface)

# add folders with function examples
# add_subdirectory(access)
add_subdirectory(basic)
add_subdirectory(busy)
add_subdirectory(matmac)
add_subdirectory(matmul)
# dependent on dlibc
# add_subdirectory(image_processing)
add_subdirectory(files)
add_subdirectory(stdio)
