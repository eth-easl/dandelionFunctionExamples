set(CMAKE_VERSION 3.13)
cmake_minimum_required(VERSION ${CMAKE_VERSION})

# prevent in-source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source build detected")
endif()

project(function_examples LANGUAGES C ASM)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../dandelionFunctionInterface/functionInterface" functionInterface)

# cheri compile flags
set(NOCAP_COMPILE_FLAGS -march=morello -mabi=aapcs)
set(NOCAP_LINK_FLAGS -march=morello -mabi=aapcs -fuse-ld=lld)

# add folders with function examples
add_subdirectory(basic)
add_subdirectory(matmul)
add_subdirectory(wrapper)
