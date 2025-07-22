#include <stdio.h>
#include "DandelionAgentLib.h"
#include "cJSON.h"
#include <bson/bson.h>

void write_http_request(const char *file_path, const char *url, const char *payload) {
    FILE *llm_request = fopen(file_path, "w+");
    fprintf(llm_request, "POST %s HTTP/1.1\n", url);
    fprintf(llm_request, "Content-Type: application/json\n\n");
    fprintf(llm_request, "%s", payload);
}

void write_http_request_from_bson(const char *file_path, const char *url, bson_t *bson_doc) {
    uint32_t bson_doc_len;
    uint8_t *data = bson_destroy_with_steal(bson_doc, true, &bson_doc_len);

    FILE *composition_request_file = fopen(file_path, "w+");
    fprintf(composition_request_file, "POST %s HTTP/1.1\n", url);
    fprintf(composition_request_file, "Content-Type: application/bson\n\n");
    fwrite(data, 1, bson_doc_len, composition_request_file);
}

int get_input_item(const char *input_path, char **item, size_t *input_len){
    FILE *input_file = fopen(input_path, "r");
    if (input_file == NULL) {
        perror("Failed to open input file.");
        return ERR_FILE_DOES_NOT_EXIST;
    }
    char *data = NULL;
    size_t data_len = 0;
    if (__getline(&data, &data_len, input_file) < 0) {
        return ERR_NO_FILE_CONTENT;
    }

    *input_len = data_len;
    *item = data;

    return SUCCESS;
}

int get_input_item_binary(const char *input_path, char **item, long *input_len){
    FILE *compositon_reply_file = fopen(input_path, "r");
    if (compositon_reply_file == NULL) {
        perror("Failed to open input file\n");
        return ERR_FILE_DOES_NOT_EXIST;
    }
    // Get file size
    fseek(compositon_reply_file, 0, SEEK_END);
    *input_len = ftell(compositon_reply_file);
    rewind(compositon_reply_file);

    // Read the file
    *item = malloc(*input_len);
    if (!*item) {
        perror("Failed to allocate buffer.");
        fclose(compositon_reply_file);
        return ERR_MEMORY_ALLOCATION;
    }
    fread(*item, 1, *input_len, compositon_reply_file);
    fclose(compositon_reply_file);
    return SUCCESS;
}

void write_output_item(const char *file_path, const char *data) {
    FILE *output_file = fopen(file_path, "w+");
    fprintf(output_file, "%s", data);
}

int extract_message_content(const char *llm_reply, const size_t llm_reply_len, char **llm_reply_content){
    cJSON *parsed_json = cJSON_ParseWithLength(llm_reply, llm_reply_len);
    if (parsed_json == NULL) {
        fprintf(stderr, "Parsing the LLM reply JSON failed.\n");
        return ERR_JSON_PARSING;
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
    } else {
        *llm_reply_content = cJSON_Print(choices);
        printf("LLM did not reply with content: %s\n", *llm_reply_content);
        return ERR_LLM_REPLY_ERROR;
    }
    return SUCCESS;
}

void get_binary_data_item_from_bson(const char *dot_path, const bson_t *bson_doc, const uint8_t **binary_data, uint32_t *binary_len){
    bson_iter_t iter, desc;
    bson_iter_init(&iter, bson_doc);
    // https://mongoc.org/libbson/current/bson_iter_find_descendant.html
    if (bson_iter_find_descendant(&iter, dot_path, &desc)) {
        bson_subtype_t subtype;
        bson_iter_binary(&desc, &subtype, binary_len, binary_data);
    }
}

void add_bson_item_data(const char * data, const char * identifier, bson_t * item){
    BSON_APPEND_UTF8(item, "identifier", identifier);
    BSON_APPEND_INT64(item, "key", 0);
    BSON_APPEND_BINARY(item, "data", BSON_SUBTYPE_BINARY, (const uint8_t *)data, strlen(data));
}

void print_as_ascii(const char * data, const size_t data_len){
    for (size_t i = 0; i < data_len; i++) {
        if (data[i] >= 32 && data[i] <= 126) {
            putchar(data[i]);
        } else {
            printf("\\x%02x", (unsigned char)data[i]);
        }
    }
    printf("\n");
}