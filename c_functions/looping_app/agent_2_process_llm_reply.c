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
  
    // Load llm reply
    FILE *llm_reply_file = fopen("/responses/llm_request", "r");
    if (llm_reply_file == NULL) {
        perror("Failed to open file with llm reply");
        return -1;
    }
    char *llm_reply = NULL;
    size_t llm_reply_len = 0;
    if (__getline(&llm_reply, &llm_reply_len, llm_reply_file) < 0) {
        perror("Failed to read line from llm reply file\n");
        return -1;
    }

    // Parse the LLM reply JSON
    cJSON *parsed_json = cJSON_ParseWithLength(llm_reply, llm_reply_len);
    if (parsed_json == NULL) {
        fprintf(stderr, "Parsing the LLM reply JSON failed.\n");
        return -1;
    }

    // Extract the content from the LLM reply
    cJSON *choices = cJSON_GetObjectItem(parsed_json, "choices");
    if (choices && cJSON_IsArray(choices)) {
        cJSON *first_choice = cJSON_GetArrayItem(choices, 0);
        cJSON *message = cJSON_GetObjectItem(first_choice, "message");
        cJSON *content = cJSON_GetObjectItem(message, "content");
        char *content_str = cJSON_PrintUnformatted(content);
        printf("LLM replied with content: %s\n", content_str);
    } else {
        char *content_str = cJSON_Print(choices);
        printf("LLM did not reply with content: %s\n", content_str);
    }
    

/* 
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
 */
  return 0;
}
