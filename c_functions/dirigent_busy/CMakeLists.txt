cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(EXECUTABLE "dirigent_busy")

add_executable(${EXECUTABLE}
  dirigent_busy.c
)

target_compile_options(${EXECUTABLE} PRIVATE -static -O3)
target_link_options(${EXECUTABLE} PRIVATE -static)
target_link_libraries(${EXECUTABLE} PRIVATE
  dlibc
  dandelion_runtime
)