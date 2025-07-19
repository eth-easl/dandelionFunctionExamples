#ifndef MYLIB_H
#define MYLIB_H

#include <bson/bson.h>  

#define SUCCESS 0
#define ERR_FILE_DOES_NOT_EXIST -1
#define ERR_NO_FILE_CONTENT -2
#define ERR_LLM_REPLY_ERROR -3
#define ERR_MEMORY_ALLOCATION -4
#define ERR_JSON_PARSING -5

/**
 * Writes a HTTP request to the given URL into a file. The request body is a JSON payload.
 *
 * @param file_path      The file path to write to, e.g., "/requests/llm_request"
 * @param url            The URL to send the request to
 * @param payload        The JSON payload to include in the request body
 */
void write_http_request(const char *file_path, const char *url, const char *payload);

/**
 * Writes a HTTP request to the given URL into a file. The request body is a BSON document.
 *
 * @param file_path      The file path to write to, e.g., "/requests/llm_request"
 * @param url            The URL to send the request to
 * @param bson_doc       The BSON document to include in the request body
 */
void write_http_request_from_bson(const char *file_path, const char *url, bson_t *bson_doc);

/**
 * Reads an input item from a file as text.
 *
 * @param input_path      The file path to read the input from, e.g., "/requests/llm_request"
 * @param item            A buffer to store the item
 * @param input_len       The length of the input item
 * @return SUCCESS on success, ERR_FILE_DOES_NOT_EXIST if the file does not exist, or ERR_NO_FILE_CONTENT if the file is empty.
 */
int get_input_item(const char *input_path, char **item, size_t *input_len);

/**
 * Reads an input item from a file as binary.
 *
 * @param input_path      The file path to read the input from, e.g., "/requests/llm_request"
 * @param item            A buffer to store the item
 * @param input_len       The length of the input item
 * @return SUCCESS on success, ERR_FILE_DOES_NOT_EXIST if the file does not exist, or ERR_MEMORY_ALLOCATION if no memory could be allocated.
 */
int get_input_item_binary(const char *input_path, char **item, long *input_len);

/**
 * Extracts the message content field of an LLM reply.
 *
 * @param llm_reply      The LLM reply JSON string
 * @param llm_reply_len  The length of the LLM reply JSON string
 * @param llm_reply_content A buffer to store the extracted message content
 * @return SUCCESS on success, ERR_JSON_PARSING if parsing the JSON fails, or ERR_LLM_REPLY_ERROR if the structure of the LLM reply is not as expected.
 */
int extract_message_content(const char *llm_reply, const size_t llm_reply_len, char **llm_reply_content);

/**
 * Gets a binary data item from a BSON document using a dot path.
 *
 * @param dot_path       The dot path to the binary data item, e.g., "sets.0.items.1.data"
 * @param bson_doc       The BSON document to search in
 * @param binary_data    A buffer to store the binary data
 * @param binary_len     The length of the binary data
 */
void get_binary_data_item_from_bson(const char *dot_path, const bson_t *bson_doc, const uint8_t **binary_data, uint32_t *binary_len);

/**
 * Formats a BSON document in the expected way for an input item.
 *
 * @param data           The data of the item
 * @param identifier     The identifier for the item
 * @param item           The BSON document to format
 */
void add_bson_item_data(const char * data, const char * identifier, bson_t * item);

/**
 * A helper function that prints binary data as ASCII characters (non-printable characters are replaced with their hex representation).
 *
 * @param data           The binary data to print
 * @param data_len       The length of the binary data
 */
void print_as_ascii(const char * data, const size_t data_len);

#endif