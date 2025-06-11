cmake_minimum_required(VERSION ${CMAKE_VERSION})

include(ExternalProject)
set(LIBZIP_INSTALL_DIR ${CMAKE_BINARY_DIR}/libzip_install)
ExternalProject_Add(libzip
  GIT_REPOSITORY https://github.com/nih-at/libzip.git
  GIT_TAG v1.11.4
  INSTALL_DIR ${LIBZIP_INSTALL_DIR}
  CMAKE_ARGS
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_BINARY_DIR}/dandelion_sdk/dandelion-toolchain.cmake
    -DBUILD_SHARED_LIBS=OFF
    -DENABLE_OPENSSL=OFF
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
)

set(EXECUTABLE "unzipper")
add_executable(${EXECUTABLE}
  unzip.c
)

target_include_directories(${EXECUTABLE} PRIVATE
  ${LIBZIP_INSTALL_DIR}/include
)

target_link_libraries(${EXECUTABLE} PRIVATE
  ${LIBZIP_INSTALL_DIR}/lib/libzip.a
  dlibc
  dandelion_runtime
)