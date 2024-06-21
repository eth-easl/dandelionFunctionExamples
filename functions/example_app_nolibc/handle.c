#include "commons.h"

int main() {
  // make sure we have the expected output sets
  if (dandelion_output_set_count() < 1) return -1;

  // read authorization server file
  if (dandelion_input_set_count() < 2) errprint("have less than 1 input set");
  if (dandelion_input_buffer_count(0) != 1)
    errprint("have no buffer in input set 0");
  if (dandelion_input_buffer_count(1) != 1)
    errprint("have no buffer in input set 1");

  struct io_buffer* server_file = dandelion_get_input(0, 0);
  char* server_data = server_file->data;
  size_t server_len = server_file->data_len;

  struct io_buffer* auth_file = dandelion_get_input(1, 0);
  char* auth_data = auth_file->data;
  size_t auth_len = auth_file->data_len;

  char* request_buffer = dandelion_alloc(2048, 8);
  size_t request_index = 0;
  char post_prefix[] = "POST http://";
  request_index += string_copy(request_buffer + request_index, post_prefix);

  // can't use stringcpy because also want to stop on line ending
  for (size_t line_index = 0;
       line_index < server_len &&
       (server_data[line_index] != '\n' || server_data[line_index] != '\0');
       line_index++) {
    request_buffer[request_index] = server_data[line_index];
    request_index++;
  }
  char post_suffix[] =
      "/authorize HTTP/1.1\nContent-Type: application/json\n\n{\"token\": \"";
  request_index += string_copy(request_buffer + request_index, post_suffix);

  // read initial data out of auth
  char auth_prefix[] = "Bearer ";
  size_t auth_index = 0;
  for (size_t prefix_index = 0; auth_prefix[prefix_index] != '\0';
       prefix_index++) {
    if (auth_data[auth_index] != auth_prefix[prefix_index]) {
      return errprint("Authprefix not in actual auth message");
    }
    auth_index++;
  }
  size_t auth_token_start = auth_index;
  while (auth_data[auth_index] != '\0' && auth_index < auth_len) {
    auth_index++;
  }
  for (size_t token_index = auth_token_start; token_index < auth_index;
       token_index++) {
    request_buffer[request_index] = auth_data[token_index];
    request_index++;
  }

  char json_suffix[] = "\"}";
  request_index += string_copy(request_buffer + request_index, json_suffix);

  struct io_buffer request = {
      .ident = "auth",
      .ident_len = 4,
      .data = request_buffer,
      .data_len = request_index,
      .key = 0,
  };
  dandelion_add_output(1, request);
  return 0;
}

DANDELION_ENTRY(main)