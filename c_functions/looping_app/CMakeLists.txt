cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(AGENT1 "agent_1_write_llm_request")
set(AGENT2 "agent_2_process_llm_reply")
set(AGENT3 "agent_3_process_composition_reply")

add_executable(${AGENT1}
  agent_1_write_llm_request.c
  cJSON/cJSON.c
)

add_executable(${AGENT2}
  agent_2_process_llm_reply.c
  cJSON/cJSON.c
  bson_shims.c
)

add_executable(${AGENT3}
  agent_3_process_composition_reply.c
  cJSON/cJSON.c
  bson_shims.c
)

# Agent 1
target_include_directories(${AGENT1} PRIVATE cJSON)

target_compile_options(${AGENT1} PRIVATE -static -Os -flto)
target_link_options(${AGENT1} PRIVATE -static -Os -flto)
target_link_libraries(${AGENT1} PRIVATE
  dlibc
  dandelion_runtime
)

# Agent 2
target_include_directories(${AGENT2} PRIVATE
  cJSON
  /workspace/dandelion_bson_install/include/libbson-1.0
)

target_compile_options(${AGENT2} PRIVATE -static -Os -flto)
target_link_options(${AGENT2} PRIVATE -static -Os -flto -L/workspace/dandelion_bson_install/lib)
target_link_libraries(${AGENT2} PRIVATE
  dlibc
  dandelion_runtime
  /workspace/dandelion_bson_install/lib/libbson-static-1.0.a
)

# Agent 3
target_include_directories(${AGENT3} PRIVATE
  cJSON
  /workspace/dandelion_bson_install/include/libbson-1.0
)

target_compile_options(${AGENT3} PRIVATE -static -Os -flto)
target_link_options(${AGENT3} PRIVATE -static -Os -flto -L/workspace/dandelion_bson_install/lib)
target_link_libraries(${AGENT3} PRIVATE
  dlibc
  dandelion_runtime
  /workspace/dandelion_bson_install/lib/libbson-static-1.0.a
)