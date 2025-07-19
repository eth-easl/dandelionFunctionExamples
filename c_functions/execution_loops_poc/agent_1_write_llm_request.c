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
  size_t llm_model_len = 0;
  size_t llm_endpoint_len = 0;
  size_t message_state_len = 0;

  int err_llm_model = get_input_item("/responses/llm_model", &llm_model, &llm_model_len);
  int err_llm_endpoint = get_input_item("/responses/llm_endpoint", &llm_endpoint, &llm_endpoint_len);
  int err_message_state = get_input_item("/responses/message_state", &message_state, &message_state_len);

  if(err_llm_endpoint != SUCCESS || err_llm_model != SUCCESS) {
    return -1;
  }
  if (err_message_state == ERR_NO_FILE_CONTENT) {
    char *prompt = NULL;
    size_t prompt_len = 0;
    get_input_item("/responses/prompt", &prompt, &prompt_len);

    // Create message object
    cJSON *message = cJSON_CreateObject();
    cJSON_AddStringToObject(message, "role", "user");
    cJSON_AddStringToObject(message, "content", prompt);

    // Create messages array
    cJSON *messages = cJSON_CreateArray();
    cJSON_AddItemToArray(messages, message);
    
    // Create full payload
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddStringToObject(payload, "model", llm_model);
    cJSON_AddItemToObject(payload, "messages", messages);
    char *payload_str = cJSON_PrintUnformatted(payload);

    printf("Payload: %s\n", payload_str);

    write_http_request("/requests/llm_request", llm_endpoint, payload_str);
  } else if (err_message_state == SUCCESS) {
    printf("Received message. Stopping after one iteration.\n");
  } else {
    return -1;
  }
  return 0;
}
