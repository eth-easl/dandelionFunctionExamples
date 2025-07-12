cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(AGENT1 "agent_1_write_llm_request")
set(AGENT2 "agent_2_process_llm_reply")

add_executable(${AGENT1}
  agent_1_write_llm_request.c
  cJSON/cJSON.c
)

add_executable(${AGENT2}
  agent_2_process_llm_reply.c
  cJSON/cJSON.c
)

target_include_directories(${AGENT1} PRIVATE cJSON)

target_compile_options(${AGENT1} PRIVATE -static -Os -flto)
target_link_options(${AGENT1} PRIVATE -static -Os -flto)
target_link_libraries(${AGENT1} PRIVATE
  dlibc
  dandelion_runtime
)

target_include_directories(${AGENT2} PRIVATE cJSON)

target_compile_options(${AGENT2} PRIVATE -static -Os -flto)
target_link_options(${AGENT2} PRIVATE -static -Os -flto)
target_link_libraries(${AGENT2} PRIVATE
  dlibc
  dandelion_runtime
)