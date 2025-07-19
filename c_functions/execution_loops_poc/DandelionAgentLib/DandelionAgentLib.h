#ifndef MYLIB_H
#define MYLIB_H

#define SUCCESS 0
#define ERR_FILE_DOES_NOT_EXIST -1
#define ERR_NO_FILE_CONTENT -2

void write_llm_request(const char *file_path, const char *llm_endpoint, const char *payload);
int get_input_item(const char *input_path, char **item, size_t *input_len);

#endif