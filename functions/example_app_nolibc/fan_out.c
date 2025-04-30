#include "commons.h"

const unsigned int SERVER_NUMBER = 10;

int main() {
  // make sure we have the expected output sets
  if (dandelion_output_set_count() < 1)
    return -7;

  // read authorization server file
  if (dandelion_input_set_count() < 2)
    errprint("have less than 2 input set");
  if (dandelion_input_buffer_count(0) != 1)
    errprint("have no buffer in input set 0");
  if (dandelion_input_buffer_count(1) != 1)
    errprint("have no buffer in input set 1");

  IoBuffer *server_file = dandelion_get_input(0, 0);
  char *server_data = server_file->data;
  size_t server_len = server_file->data_len;

  IoBuffer *auth_file = dandelion_get_input(1, 0);
  char *auth_data = auth_file->data;
  size_t auth_len = auth_file->data_len;

  // find username
  size_t user_start = 0;
  char user_prefix[] = "{\"authorized\":\"";
  for (size_t prefix_index = user_start; user_prefix[prefix_index] != '\0';
       prefix_index++) {
    if (auth_data[user_start] != user_prefix[prefix_index]) {
      return errprint("user_prefix not in actual auth message");
    }
    user_start++;
  }

  size_t user_end = user_start;
  while (auth_data[user_end] != '\"' && user_end < auth_len) {
    user_end++;
  }

  if (SERVER_NUMBER > 99)
    return errprint("More than 99 servers, need different formatting");
  for (size_t server_index = 0; server_index < SERVER_NUMBER; server_index++) {
    char *request_buffer = dandelion_alloc(2048, 8);
    size_t request_index = 0;
    char *server_name = dandelion_alloc(16, 8);
    char server_prefix[] = "server_";
    int server_name_end = string_copy(server_name, server_prefix);
    size_t decimal = server_index / 10;
    size_t digit = server_index - 10 * decimal;
    server_name[server_name_end] = CHAR_NUMBERS[decimal];
    server_name_end++;
    server_name[server_name_end] = CHAR_NUMBERS[digit];
    server_name_end++;

    char get_prefix[] = "GET http://";
    request_index += string_copy(request_buffer + request_index, get_prefix);

    for (size_t line_index = 0;
         line_index < server_len &&
         (server_data[line_index] != '\n' || server_data[line_index] != '\0');
         line_index++) {
      request_buffer[request_index] = server_data[line_index];
      request_index++;
    }

    char log_string[] = "/logs/";
    request_index += string_copy(request_buffer + request_index, log_string);

    request_buffer[request_index] = CHAR_NUMBERS[decimal];
    request_index++;
    request_buffer[request_index] = CHAR_NUMBERS[digit];
    request_index++;

    char get_suffix[] = " HTTP/1.1\n\n{\"username\": \"";
    request_index += string_copy(request_buffer + request_index, get_suffix);

    // copy username
    for (size_t user_index = user_start; user_index < user_end; user_index++) {
      request_buffer[request_index] = auth_data[user_index];
      request_index++;
    }

    char json_suffix[] = "\"}";
    request_index += string_copy(request_buffer + request_index, json_suffix);

    IoBuffer request = {
        .ident = server_name,
        .ident_len = server_name_end,
        .data = request_buffer,
        .data_len = request_index,
        .key = 0,
    };
    dandelion_add_output(1, request);
  }

  return 0;
}

DANDELION_ENTRY(main)