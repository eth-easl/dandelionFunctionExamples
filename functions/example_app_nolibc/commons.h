#include <stdint.h>

#include "dandelion/crt.h"
#include "dandelion/runtime.h"

const char CHAR_NUMBERS[10] = {'0', '1', '2', '3', '4',
                               '5', '6', '7', '8', '9'};

int string_copy(char* target, char* source) {
  size_t read_chars = 0;
  while (source[read_chars] != '\0') {
    target[read_chars] = source[read_chars];
    read_chars++;
  }
  return read_chars;
}

int copy_to_string_end(char* target, char* source) {
  size_t read_chars = 0;
  while (source[read_chars] != '\"') {
    target[read_chars] = source[read_chars];
    read_chars++;
  }
  target[read_chars] = '\\';
  return read_chars;
}

int skip_string(char* target, char* comparison) {
  size_t read_chars = 0;
  while (comparison[read_chars] != '\0') {
    if (comparison[read_chars] != target[read_chars]) return -1;
    read_chars++;
  }
  return read_chars;
}

int errprint(char* message) {
  if (dandelion_output_set_count() < 1) {
    return -99;
  }
  char* stderr = dandelion_alloc(128, 8);
  size_t message_index = 0;
  for (; message[message_index] != '\0' && message_index < 127;
       message_index++) {
    stderr[message_index] = message[message_index];
  }
  stderr[message_index] = '\0';

  const char stderr_ident[] = "stderr";
  struct io_buffer stderr_buffer = {
      .ident = stderr_ident,
      .ident_len = 7,
      .data = stderr,
      .data_len = message_index + 1,
      .key = 0,
  };

  dandelion_add_output(0, stderr_buffer);

  return -1;
}