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

  // Load llm model
  FILE *llm_model_file = fopen("/responses/llm_model", "r");
  if (llm_model_file == NULL) {
    perror("Failed to open file with llm model");
    return -1;
  }
  char *llm_model = NULL;
  size_t llm_model_len = 0;
  if (__getline(&llm_model, &llm_model_len, llm_model_file) < 0) {
    perror("Failed to read line from llm model file\n");
    return -1;
  }

  // Load llm endpoint
  FILE *llm_endpoint_file = fopen("/responses/llm_endpoint", "r");
  if (llm_endpoint_file == NULL) {
    perror("Failed to open file to get llm endpoint");
    return -1;
  }
  char *llm_endpoint = NULL;
  size_t llm_endpoint_len = 0;
  if (__getline(&llm_endpoint, &llm_endpoint_len, llm_endpoint_file) < 0) {
    perror("Failed to read line from llm endpoint file\n");
    return -1;
  }

  // Load message state
  FILE *message_state_file = fopen("/responses/message_state", "r");
  if (message_state_file == NULL) {
    perror("Failed to open file to get message state");
    return -1;
  }
  char *message_state = NULL;
  size_t message_state_len = 0;
  if (__getline(&message_state, &message_state_len, message_state_file) < 0) {
    printf("No message received yet. Starting conversation with user prompt.\n");
    // Load prompt
    FILE *prompt_file = fopen("/responses/prompt", "r");
    if (prompt_file == NULL) {
      perror("Failed to open file to get prompt");
      return -1;
    }
    char *prompt = NULL;
    size_t prompt_len = 0;
    if (__getline(&prompt, &prompt_len, prompt_file) < 0) {
      perror("Failed to read line from prompt file\n");
      return -1;
    }
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

    write_llm_request("/requests/llm_request", llm_endpoint, payload_str);
  }else{
    printf("Received message. Stopping after one iteration.\n");
  }

  return 0;
}
