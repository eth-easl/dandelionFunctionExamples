#include "commons.h"

typedef struct log_node {
  struct log_node *next;
  char timestamp[100];
  char server_id[100];
  char type[100];
  char details[100];
} log_node;

int cmp_timestamp(char *time_1, char *time_2) {
  size_t index = 0;
  while (time_1[index] != '\0' && time_2[index] != '\0') {
    if (time_1[index] > time_2[index])
      return 1;
    if (time_1[index] < time_2[index])
      return -1;
    index++;
  }
  return 0;
}

const size_t MAX_LOG_SIZE = 1048576;

int main() {
  // make sure we have the expected output sets
  if (dandelion_output_set_count() < 1)
    return -7;

  log_node *log_root = NULL;

  // read authorization server file
  if (dandelion_input_set_count() < 2)
    errprint("have less than 2 input set");
  size_t log_number = dandelion_input_buffer_count(1);
  for (size_t log_index = 0; log_index < log_number; log_index++) {
    // read the file until the first '['
    IoBuffer *log_file = dandelion_get_input(1, log_index);
    char *log_data = log_file->data;
    size_t log_len = log_file->data_len;
    size_t log_index = 0;
    while (log_index < log_len && log_data[log_index] != '[') {
      log_index++;
    }
    // skip the '['
    log_index++;

    while (log_index < log_len) {
      log_node *new_node = dandelion_alloc(sizeof(log_node), 8);
      int chars_read;
      if ((chars_read = skip_string(log_data + log_index, "{\"details\":\"")) <
          0) {
        errprint("failed to read details");
        return errprint(log_data + log_index);
      } else {
        log_index += chars_read;
      }
      log_index += copy_to_string_end(new_node->details, log_data + log_index);

      if ((chars_read =
               skip_string(log_data + log_index, "\",\"event_type\":\"")) < 0) {
        errprint("failed to read event type");
        return errprint(log_data + log_index);
      } else {
        log_index += chars_read;
      }
      log_index += copy_to_string_end(new_node->type, log_data + log_index);

      if ((chars_read =
               skip_string(log_data + log_index, "\",\"server_id\":\"")) < 0) {
        errprint("failed to read event server id");
        return errprint(log_data + log_index);
      } else {
        log_index += chars_read;
      }
      log_index +=
          copy_to_string_end(new_node->server_id, log_data + log_index);

      if ((chars_read =
               skip_string(log_data + log_index, "\",\"timestamp\":\"")) < 0) {
        errprint("failed to read event server id");
        return errprint(log_data + log_index);
      } else {
        log_index += chars_read;
      }
      log_index +=
          copy_to_string_end(new_node->timestamp, log_data + log_index);
      // skip to next { or to end
      while (log_index < log_len && log_data[log_index] != '{') {
        log_index++;
      }
      if (log_root == NULL) {
        log_root = new_node;
      } else {
        if (cmp_timestamp(new_node->timestamp, log_root->timestamp) <= 0) {
          new_node->next = log_root;
          log_root = new_node;
        } else {
          log_node *previous = log_root;
          while (previous->next != NULL) {
            log_node *tmp = previous->next;
            if (cmp_timestamp(new_node->timestamp, tmp->timestamp) <= 0) {
              new_node->next = tmp;
              break;
            }
            previous = tmp;
          }
          previous->next = new_node;
        }
      }
    }
  }
  char *request_buffer = dandelion_alloc(MAX_LOG_SIZE, 8);
  size_t request_index = 0;

  char preamble[] = "<!DOCTYPE html>\n"
                    "<html lang=\"en\">\n"
                    "<head>\n"
                    "    <meta charset=\"UTF-8\">\n"
                    "    <title>Logs</title>\n"
                    "</head>\n"
                    "<body>\n"
                    "    <table>\n"
                    "        <tr>\n"
                    "            <th>Timestamp</th>\n"
                    "            <th>Server ID</th>\n"
                    "            <th>Event Type</th>\n"
                    "            <th>Details</th>\n"
                    "        </tr>\n";
  request_index += string_copy(request_buffer + request_index, preamble);

  log_node *current = log_root;
  while (current != NULL) {
    request_index += string_copy(request_buffer + request_index,
                                 "        <tr>\n            <th>");
    request_index +=
        string_copy(request_buffer + request_index, current->timestamp);
    request_index +=
        string_copy(request_buffer + request_index, "</th>\n            <th>");
    request_index +=
        string_copy(request_buffer + request_index, current->server_id);
    request_index +=
        string_copy(request_buffer + request_index, "</th>\n            <th>");
    request_index += string_copy(request_buffer + request_index, current->type);
    request_index +=
        string_copy(request_buffer + request_index, "</th>\n            <th>");
    request_index +=
        string_copy(request_buffer + request_index, current->details);
    request_index +=
        string_copy(request_buffer + request_index, "</th>\n        </tr>\n");
    current = current->next;
  }

  request_index +=
      string_copy(request_buffer + request_index, "    </table>\n</body>\n");

  IoBuffer request = {
      .ident = "log.txt",
      .ident_len = 7,
      .data = request_buffer,
      .data_len = request_index,
      .key = 0,
  };
  dandelion_add_output(1, request);

  return 0;
}

DANDELION_ENTRY(main)
