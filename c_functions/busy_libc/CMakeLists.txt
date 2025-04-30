cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(EXECUTABLE "busy_libc")

add_executable(${EXECUTABLE}
  busy_libc.c
)

target_compile_options(${EXECUTABLE} PRIVATE -static -Os -flto)
target_link_options(${EXECUTABLE} PRIVATE -static -Os -flto)
target_link_libraries(${EXECUTABLE} PRIVATE
  dlibc
  dandelion_runtime
)