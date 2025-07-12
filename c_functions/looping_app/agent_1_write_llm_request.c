#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "cJSON.h"

#include "unistd.h"

int main(int argc, char const *argv[]) {

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

  printf("LLM Endpoint: %s\n", llm_endpoint);
  printf("LLM Model: %s\n", llm_model);

  // Create message object
  cJSON *message = cJSON_CreateObject();
  cJSON_AddStringToObject(message, "role", "user");
  cJSON_AddStringToObject(message, "content", "Can you help me with something?");

  // Create messages array
  cJSON *messages = cJSON_CreateArray();
  cJSON_AddItemToArray(messages, message);
  
  // Create full payload
  cJSON *payload = cJSON_CreateObject();
  cJSON_AddStringToObject(payload, "model", llm_model);
  cJSON_AddItemToObject(payload, "messages", messages);
  char *payload_str = cJSON_PrintUnformatted(payload);

  printf("Payload: %s\n", payload_str);

  // Write LLM request
  FILE *llm_request = fopen("/requests/llm_request", "w+");
  fprintf(llm_request, "POST %s HTTP/1.1\n", llm_endpoint);
  fprintf(llm_request, "Content-Type: application/json\n\n");
  fprintf(llm_request, "%s", payload_str);

  return 0;
}
