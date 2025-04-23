cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(EXECUTABLE "tokenizer")
add_executable(${EXECUTABLE}
  test.c
)

target_link_libraries(${EXECUTABLE} PRIVATE
  dlibc
  dandelion_runtime
)