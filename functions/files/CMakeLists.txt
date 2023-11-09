cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(EXECUTABLE "fileio")

add_executable(${EXECUTABLE}
  fileio.c
)

add_dependencies(${EXECUTABLE} dlibc)

target_compile_options(${EXECUTABLE} PRIVATE -static -O0)
target_link_options(${EXECUTABLE} PRIVATE -static)
target_link_libraries(${EXECUTABLE} PRIVATE
  dlibc
)