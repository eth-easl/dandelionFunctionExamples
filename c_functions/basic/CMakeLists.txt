cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(EXECUTABLE "basic")
add_executable(${EXECUTABLE}
  basic.c
)

target_compile_options(${EXECUTABLE} PRIVATE -fno-stack-protector -O0)
target_link_options(${EXECUTABLE} PRIVATE -static)
target_link_libraries(${EXECUTABLE} PRIVATE dandelion_runtime)
