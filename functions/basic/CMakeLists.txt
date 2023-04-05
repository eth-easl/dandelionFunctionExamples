cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(EXECUTABLE "basic")
add_executable(${EXECUTABLE}
  basic.c
)

target_compile_options(${EXECUTABLE} PRIVATE ${NOCAP_COMPILE_FLAGS} -static -O0)
target_link_options(${EXECUTABLE} PRIVATE ${NOCAP_LINK_FLAGS} -static -fno-builtin -nostdlib -e main)
target_link_libraries(${EXECUTABLE} PRIVATE ioLib)
