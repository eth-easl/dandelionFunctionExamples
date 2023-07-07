cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(EXECUTABLE "stdio")

add_executable(${EXECUTABLE}
  stdio.c
)

target_compile_options(${EXECUTABLE} PRIVATE ${NOCAP_COMPILE_FLAGS} -static -O0)
target_link_options(${EXECUTABLE} PRIVATE ${NOCAP_LINK_FLAGS} -static)
message(STATUS ${MLIBC})
target_link_libraries(${EXECUTABLE} PRIVATE
  ${MLIBC}
)