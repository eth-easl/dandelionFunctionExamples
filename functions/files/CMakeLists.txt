cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(EXECUTABLE "fileio")

add_executable(${EXECUTABLE}
  fileio.c
)

add_dependencies(${EXECUTABLE} mlibc)

target_compile_options(${EXECUTABLE} PRIVATE ${NOCAP_COMPILE_FLAGS} -static -O0)
target_link_options(${EXECUTABLE} PRIVATE ${NOCAP_LINK_FLAGS} -static)
target_link_libraries(${EXECUTABLE} PRIVATE
  dlibc
)