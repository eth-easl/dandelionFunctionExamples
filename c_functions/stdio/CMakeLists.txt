cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(EXECUTABLE "stdio")

add_executable(${EXECUTABLE}
  stdio.c
)

target_link_libraries(${EXECUTABLE} PRIVATE
  dlibc
  dandelion_runtime
)