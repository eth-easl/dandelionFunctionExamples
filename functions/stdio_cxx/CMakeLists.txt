cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(EXECUTABLE "stdio_cxx")

add_executable(${EXECUTABLE}
  stdio.cpp
)

target_link_libraries(${EXECUTABLE} PRIVATE
  dlibcxx
  dlibc
  dandelion_runtime
)