cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(EXECUTABLE "matmac")

add_executable(${EXECUTABLE}
  matmac.c
)

target_compile_options(${EXECUTABLE} PRIVATE ${NOCAP_COMPILE_FLAGS} -fno-stack-protector -O3)
target_link_options(${EXECUTABLE} PRIVATE ${NOCAP_LINK_FLAGS} -static  )
target_link_libraries(${EXECUTABLE} PRIVATE dandelion_runtime compiler_lib)
