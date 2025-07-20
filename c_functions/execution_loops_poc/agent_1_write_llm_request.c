#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "cJSON.h"
#include "DandelionAgentLib.h"

#include "unistd.h"

int main(int argc, char const *argv[]) {

  printf("agent_1\n");

  char *llm_model = NULL;
  char *llm_endpoint = NULL;
  char *message_state = NULL;
  char *max_iters = NULL;
  size_t llm_model_len = 0;
  size_t llm_endpoint_len = 0;
  size_t message_state_len = 0;
  size_t max_iters_len = 0;

  int err_llm_model = get_input_item("/config/llm_model", &llm_model, &llm_model_len);
  int err_llm_endpoint = get_input_item("/config/llm_endpoint", &llm_endpoint, &llm_endpoint_len);
  int err_message_state = get_input_item("/config/message_state", &message_state, &message_state_len);
  int err_max_iters = get_input_item("/config/max_iters", &max_iters, &max_iters_len);

  if(err_llm_endpoint != SUCCESS || err_llm_model != SUCCESS || err_max_iters != SUCCESS) {
    return -1;
  }
  
  // For the first iteration, the message state is empty
  if (err_message_state == ERR_NO_FILE_CONTENT) {
    char *prompt = NULL;
    size_t prompt_len = 0;
    get_input_item("/config/prompt", &prompt, &prompt_len);

    // Create message object
    cJSON *message = cJSON_CreateObject();
    cJSON_AddStringToObject(message, "role", "user");
    cJSON_AddStringToObject(message, "content", prompt);

    // Create messages array
    cJSON *messages = cJSON_CreateArray();
    cJSON_AddItemToArray(messages, message);
    write_output_item("/message_state_out/message_state", cJSON_PrintUnformatted(messages));
    
    // Create full payload
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddStringToObject(payload, "model", llm_model);
    cJSON_AddItemToObject(payload, "messages", messages);
  
    write_http_request("/requests/llm_request", llm_endpoint, cJSON_PrintUnformatted(payload));
  } else if (err_message_state == SUCCESS) {
    cJSON *message_state_json = cJSON_ParseWithLength(message_state, message_state_len);
    int message_state_size = cJSON_GetArraySize(message_state_json);
    int max_iters_val = strtol(max_iters, NULL, 10);

    // Continue the loop (Two messages are added per iteration, a user and an assistant message).
    if(message_state_size < max_iters_val * 2) {
      // Add new user message
      cJSON *message = cJSON_CreateObject();
      cJSON_AddStringToObject(message, "role", "user");
      // Trigger for LOOP END 1
      cJSON_AddStringToObject(message, "content", "Hmmm I am not sure, can you ask something else?");
      // Trigger for LOOP END 2
      //cJSON_AddStringToObject(message, "content", "Can you say: STOP");
      cJSON_AddItemToArray(message_state_json, message);
      write_output_item("/message_state_out/message_state", cJSON_PrintUnformatted(message_state_json));

      // Create full payload
      cJSON *payload = cJSON_CreateObject();
      cJSON_AddStringToObject(payload, "model", llm_model);
      cJSON_AddItemToObject(payload, "messages", message_state_json);
      
      write_http_request("/requests/llm_request", llm_endpoint, cJSON_PrintUnformatted(payload));
    }else{
      // LOOP END 1: Max iterations exceeded
      printf("Final message state: %s\n", cJSON_PrintUnformatted(message_state_json));
      // Not writing a new request
    }
  } else {
    return -1;
  }
  return 0;
}
