cmake_minimum_required(VERSION ${CMAKE_VERSION})

add_subdirectory(linux_syscalls)

set(EXECUTABLE "busy_hybrid")

add_executable(${EXECUTABLE}
  busy_hybrid.c
)

target_compile_options(${EXECUTABLE} PRIVATE -static -Os -flto)
target_link_options(${EXECUTABLE} PRIVATE -static -Os -flto)
target_link_libraries(${EXECUTABLE} PRIVATE
  linux_syscalls
  dlibc
  dandelion_runtime
)