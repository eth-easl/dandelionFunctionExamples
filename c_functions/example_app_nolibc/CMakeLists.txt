cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(HANDLE "handle_nolibc")
set(FAN_OUT "fan_out_nolibc")
set(TEMPLATE "template_nolibc")

add_executable(${HANDLE}
  handle.c
)
add_executable(${FAN_OUT}
  fan_out.c
)
add_executable(${TEMPLATE}
  template.c
)

target_compile_options(${HANDLE} PRIVATE -static -O3)
target_link_options(${HANDLE} PRIVATE -static)
target_link_libraries(${HANDLE} PRIVATE
dandelion_runtime
)

target_compile_options(${FAN_OUT} PRIVATE -static -O3)
target_link_options(${FAN_OUT} PRIVATE -static)
target_link_libraries(${FAN_OUT} PRIVATE
dandelion_runtime
)

target_compile_options(${TEMPLATE} PRIVATE -static -O3)
target_link_options(${TEMPLATE} PRIVATE -static)
target_link_libraries(${TEMPLATE} PRIVATE
dandelion_runtime
)