#include <stdio.h>
#include "DandelionAgentLib.h"
#include "cJSON.h"

void write_llm_request(const char *file_path, const char *llm_endpoint, const char *payload) {
    FILE *llm_request = fopen("/requests/llm_request", "w+");
    fprintf(llm_request, "POST %s HTTP/1.1\n", llm_endpoint);
    fprintf(llm_request, "Content-Type: application/json\n\n");
    fprintf(llm_request, "%s", payload);
}

int get_input_item(const char *input_path, char **item, size_t *input_len){
    FILE *input_file = fopen(input_path, "r");
    if (input_file == NULL) {
        perror("Failed to open input file");
        return ERR_FILE_DOES_NOT_EXIST;
    }
    char *data = NULL;
    size_t data_len = 0;
    if (__getline(&data, &data_len, input_file) < 0) {
        perror("Failed to read line from input file\n");
        return ERR_NO_FILE_CONTENT;
    }

    *input_len = data_len;
    *item = data;

    return SUCCESS;
}

int extract_message_content(const char *llm_reply, const size_t llm_reply_len, char **llm_reply_content){
    // Parse the LLM reply JSON
    cJSON *parsed_json = cJSON_ParseWithLength(llm_reply, llm_reply_len);
    if (parsed_json == NULL) {
        fprintf(stderr, "Parsing the LLM reply JSON failed.\n");
        return -1;
    }

    *llm_reply_content = NULL;
    cJSON *choices = cJSON_GetObjectItem(parsed_json, "choices");

    if (choices && cJSON_IsArray(choices)) {
        cJSON *first_choice = cJSON_GetArrayItem(choices, 0);
        if(!first_choice) {
            return ERR_LLM_REPLY_ERROR;
        }
        cJSON *message = cJSON_GetObjectItem(first_choice, "message");
        if(!message) {
            return ERR_LLM_REPLY_ERROR;
        }
        cJSON *content = cJSON_GetObjectItem(message, "content");
        if(!content) {
            return ERR_LLM_REPLY_ERROR;
        }
        *llm_reply_content = cJSON_PrintUnformatted(content);
        printf("LLM replied with content: %s\n", llm_reply_content);
    } else {
        *llm_reply_content = cJSON_Print(choices);
        printf("LLM did not reply with content: %s\n", llm_reply_content);
        return ERR_LLM_REPLY_ERROR;
    }
    return SUCCESS;
}