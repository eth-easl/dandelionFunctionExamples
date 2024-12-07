cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(EXECUTABLE "compression")

add_executable(${EXECUTABLE}
  compression.c
)

target_compile_options(${EXECUTABLE} PRIVATE -fno-stack-protector -O3)
target_link_options(${EXECUTABLE} PRIVATE -static -O3)
target_link_libraries(${EXECUTABLE} PRIVATE dandelion_runtime)
