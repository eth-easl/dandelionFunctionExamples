cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(EXECUTABLE "app_bybrid")

add_executable(${EXECUTABLE}
app_hybrid.c
)

target_compile_options(${EXECUTABLE} PRIVATE -static -O0)
target_link_options(${EXECUTABLE} PRIVATE -static)
target_link_libraries(${EXECUTABLE} PRIVATE
  dlibc
)