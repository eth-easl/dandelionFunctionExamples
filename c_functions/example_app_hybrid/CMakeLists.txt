cmake_minimum_required(VERSION ${CMAKE_VERSION})

add_subdirectory(linux_syscalls)

set(EXECUTABLE "app_hybrid")

add_executable(${EXECUTABLE}
app_hybrid.c
)

target_compile_options(${EXECUTABLE} PRIVATE -static -Os -flto)
target_link_options(${EXECUTABLE} PRIVATE -static -Os -flto)
target_link_libraries(${EXECUTABLE} PRIVATE
  linux_syscalls
  dlibc
  dandelion_runtime
)