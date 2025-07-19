#ifndef MYLIB_H
#define MYLIB_H

#include <bson/bson.h>  

#define SUCCESS 0
#define ERR_FILE_DOES_NOT_EXIST -1
#define ERR_NO_FILE_CONTENT -2
#define ERR_LLM_REPLY_ERROR -3

void write_http_request(const char *file_path, const char *llm_endpoint, const char *payload);
void write_bson_http_request(const char *file_path, const char *url, bson_t *doc);
int get_input_item(const char *input_path, char **item, size_t *input_len);
int get_input_item_binary(const char *input_path, char **item, long *input_len);
int extract_message_content(const char *llm_reply, const size_t llm_reply_len, char **llm_reply_content);
void add_bson_item_data(const char * data, const char * identifier, bson_t * item);
void print_as_ascii(const char * data, const size_t data_len);
void get_binary_data_item_from_bson(const char *dot_path, const bson_t *bson_doc, const uint8_t **binary_data, uint32_t *binary_len);

#endif