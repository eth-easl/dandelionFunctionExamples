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

target_compile_options(${HANDLE} PRIVATE -static -Os -flto)
target_link_options(${HANDLE} PRIVATE -static -Os -flto)
target_link_libraries(${HANDLE} PRIVATE
  dlibc
  dandelion_runtime
)

target_compile_options(${FAN_OUT} PRIVATE -static -Os -flto)
target_link_options(${FAN_OUT} PRIVATE -static -Os -flto)
target_link_libraries(${FAN_OUT} PRIVATE
  dlibc
  dandelion_runtime
)

target_compile_options(${TEMPLATE} PRIVATE -static -Os -flto)
target_link_options(${TEMPLATE} PRIVATE -static -Os -flto)
target_link_libraries(${TEMPLATE} PRIVATE
  dlibc
  dandelion_runtime
)