#include <stdio.h>
#include "DandelionAgentLib.h"

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