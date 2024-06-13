cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(HANDLE "handle")
set(FAN_OUT "fan_out")
set(TEMPLATE "template")

add_executable(${HANDLE}
  handle.c
)
add_executable(${FAN_OUT}
  fan_out.c
)
add_executable(${TEMPLATE}
  template.c
)

add_dependencies(${HANDLE} dlibc)
add_dependencies(${FAN_OUT} dlibc)
add_dependencies(${TEMPLATE} dlibc)

target_compile_options(${HANDLE} PRIVATE -static -O3)
target_link_options(${HANDLE} PRIVATE -static)
target_link_libraries(${HANDLE} PRIVATE
  dlibc
)

target_compile_options(${FAN_OUT} PRIVATE -static -O3)
target_link_options(${FAN_OUT} PRIVATE -static)
target_link_libraries(${FAN_OUT} PRIVATE
  dlibc
)

target_compile_options(${TEMPLATE} PRIVATE -static -O3)
target_link_options(${TEMPLATE} PRIVATE -static)
target_link_libraries(${TEMPLATE} PRIVATE
  dlibc
)