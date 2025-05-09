#include "unistd.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "syscall.h"

#define BUFFER_SIZE 4096
#define SERVER_NUMBER 10
const uint16_t AUTH_SERVER_PORT = 8000;

// Structure to store log event details
typedef struct log_node {
  struct log_node *next;
  char timestamp[100];
  char server_id[100];
  char type[100];
  char details[100];
} log_node;

char event_template[] = "%*[^{]{"
                        "\"details\":\"%100[^\"]\","
                        "\"event_type\":\"%100[^\"]\","
                        "\"server_id\":\"%100[^\"]\","
                        "\"timestamp\":\"%100[^\"]\""
                        "%}";

uint16_t my_htons(uint16_t port) {
  return ((port >> 8) & 0x00FF) | ((port << 8) & 0xFF00);
}

int my_inet_pton(int af, const char *src, void *dst) {
  if (af == LINUX_AF_INET) {
    uint8_t array[4] = {};
    for (int i = 0; i < 4; i++) {
      char *end;
      long value = strtol(src, &end, 10);
      if (value < 0 || value > 255) {
        return 0;
      }

      if (*end != '\0' && *end != '.' && i < 3) {
        return 0;
      }
      src = end + 1;
      array[i] = (uint8_t)value;
    }

    struct in_addr *addr = (struct in_addr *)dst;
    memcpy(&addr->s_addr, array, 4); // Copy the 4 bytes into s_addr

    return 1; // Success
  }
  return -1; // Unsupported address family
}

int connect_to_server(const char *ip, uint16_t port) {
  struct sockaddr_in server_addr;

  int sockfd = linux_socket(LINUX_AF_INET, LINUX_SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("Socket creation failed");
    return -1;
  }

  server_addr.sin_family = LINUX_AF_INET;
  server_addr.sin_port = my_htons(port);

  if (my_inet_pton(LINUX_AF_INET, ip, &server_addr.sin_addr) != 1) {
    perror("Invalid IP address");
    close(sockfd);
    return -1;
  }

  int err = linux_connect(sockfd, &server_addr, sizeof(server_addr));
  if (err < 0) {
    printf("Error: %s\n", strerror(-err));
    printf("size of server_addr: %ld\n", sizeof(server_addr));
    perror("Socket connection failed");
    close(sockfd);
    return -1;
  }

  return sockfd;
}

int send_http_request(int sockfd, const char *request, size_t request_size,
                      char *response, size_t response_size) {
  if (linux_send(sockfd, request, request_size, 0) < 0) {
    perror("Send failed");
    return -1;
  }

  ssize_t total_received = 0, bytes;
  // while ((bytes = linux_recv(sockfd, response + total_received, response_size
  // - total_received - 1, 0)) > 0) {
  //     total_received += bytes;
  //     if (total_received >= response_size - 1) break;
  // }
  // blocks and waits forever if there is no next packate, could validate HTTP
  // sice on the packages to see if we need more, this works for now
  total_received = linux_recv(sockfd, response + total_received,
                              response_size - total_received - 1, 0);
  if (total_received < 0) {
    return -1;
  }
  response[total_received] = '\0';
  return 0;
}

void parse_auth_response(const char *response, char *token) {
  const char *start = strstr(response, "\"token\":");
  if (start) {
    sscanf(start, "\"token\": \"%[^\"]", token);
  }
}

void parse_log_events(const char *response, log_node **log_root) {
  log_node *current = NULL;

  const char *start = strstr(response, "\"events\":");
  if (start) {
    start += 9;
    while (1) {
      current = malloc(sizeof(log_node));
      int found = sscanf(start, event_template, current->details, current->type,
                         current->server_id, current->timestamp);
      if (found != 4) {
        free(current);
        break;
      }

      current->next = *log_root;
      *log_root = current;

      start = strstr(start, "},{");
      if (!start)
        break;
      start += 1;
    }
  }
}

// Render logs to an HTML file
void render_logs_to_html(log_node *log_root) {
  FILE *log_file = fopen("/requests/log.html", "w+");
  if (!log_file) {
    perror("Failed to open HTML file");
    return;
  }

  // sort by date
  // get number
  size_t num_entries = 0;
  log_node *current = log_root;
  while (current) {
    num_entries++;
    current = current->next;
  }
  log_node **log_heap = malloc(sizeof(log_node *) * num_entries);
  current = log_root;
  for (size_t index = 0; index < num_entries; index++) {
    log_heap[index] = current;
    current = current->next;
    for (size_t insert = index; insert > 0; insert = insert / 2) {
      if (strcmp(log_heap[insert / 2]->timestamp,
                 log_heap[insert]->timestamp) <= 0) {
        break;
      } else {
        log_node *tmp = log_heap[insert];
        log_heap[insert] = log_heap[insert / 2];
        log_heap[insert / 2] = tmp;
      }
    }
  }

  // write preamble
  fprintf(log_file, "<!DOCTYPE html>\n");
  fprintf(log_file, "<html lang=\"en\">\n");
  fprintf(log_file, "<head>\n");
  fprintf(log_file, "    <meta charset=\"UTF-8\">\n");
  fprintf(log_file, "    <title>Logs</title>\n");
  fprintf(log_file, "</head>\n");
  fprintf(log_file, "<body>\n");
  fprintf(log_file, "    <table>\n");
  fprintf(log_file, "        <tr>\n");
  fprintf(log_file, "            <th>Timestamp</th>\n");
  fprintf(log_file, "            <th>Server ID</th>\n");
  fprintf(log_file, "            <th>Event Type</th>\n");
  fprintf(log_file, "            <th>Details</th>\n");
  fprintf(log_file, "        </tr>\n");

  // write log information
  for (; num_entries > 0; num_entries--) {
    current = log_heap[0];
    fprintf(log_file, "        <tr>\n");
    fprintf(log_file, "            <th>%s</th>\n", current->timestamp);
    fprintf(log_file, "            <th>%s</th>\n", current->server_id);
    fprintf(log_file, "            <th>%s</th>\n", current->type);
    fprintf(log_file, "            <th>%s</th>\n", current->details);
    fprintf(log_file, "        </tr>\n");
    // remove current root from heap
    size_t index = 0;
    log_heap[0] = log_heap[num_entries - 1];
    // trade down until no more children
    while (index * 2 + 1 < num_entries) {
      // compare with first child
      int is_bigger = strcmp(log_heap[index]->timestamp,
                             log_heap[index * 2 + 1]->timestamp);
      // check if second child exists
      if (index * 2 + 2 >= num_entries) {
        // second child does not exist, if is bigger than child swap
        if (is_bigger > 0) {
          log_node *tmp = log_heap[index];
          log_heap[index] = log_heap[index * 2 + 1];
          log_heap[index * 2 + 1] = tmp;
        }
        break;
      }
      // know fist child is smaller, if second is smaller than first swap with
      // second otherwise with first
      if (is_bigger > 0) {
        size_t swap_index;
        if (strcmp(log_heap[index * 2 + 1]->timestamp,
                   log_heap[index * 2 + 2]->timestamp) <= 0) {
          // first is smaller
          swap_index = index * 2 + 1;
        } else {
          swap_index = index * 2 + 2;
        }
        log_node *tmp = log_heap[index];
        log_heap[index] = log_heap[swap_index];
        log_heap[swap_index] = tmp;
        index = swap_index;
      } else {
        // first is not smaller so need to check second if swap is neccessary
        if (strcmp(log_heap[index]->timestamp,
                   log_heap[index * 2 + 2]->timestamp) > 0) {
          // is bigger so need to swap
          log_node *tmp = log_heap[index];
          log_heap[index] = log_heap[index * 2 + 2];
          log_heap[index * 2 + 2] = tmp;
          index = index * 2 + 2;
        } else {
          break;
        }
      }
    }
  }

  // write ending
  fprintf(log_file, "    </table>\n");
  fprintf(log_file, "</body>\n");

  fclose(log_file);
}

// Main function to handle authorization, log fetching, and rendering
int main() {
  // read authentification server ip from file
  FILE *auth_server = fopen("/servers/server.txt", "r");
  if (auth_server == NULL) {
    perror("Failed to open file to get auth server");
    return -1;
  }
  char *auth_server_ip = NULL;
  size_t server_line_len = 0;
  size_t read_chars = __getline(&auth_server_ip, &server_line_len, auth_server);
  if (read_chars < 0) {
    perror("Fauled to read line from auth server file\n");
    return -1;
  }
  // strip possible endln from the getline
  if (auth_server_ip[read_chars - 1] == '\n') {
    auth_server_ip[read_chars - 1] = '\0';
  }

  // read token from incomming file
  FILE *token_file = fopen("/responses/Authorization", "r");
  if (token_file == NULL) {
    perror("Failed to open file with token");
    return -1;
  }
  char auth_token[257] = {0};
  if (fscanf(token_file, "Bearer %256s", auth_token) < 0) {
    perror("Failed to parse line from token file");
    return -1;
  }

  // handle
  int auth_sock = connect_to_server(auth_server_ip, AUTH_SERVER_PORT);
  if (auth_sock < 0)
    return 1;

  char auth_request[BUFFER_SIZE];
  int written = snprintf(auth_request, sizeof(auth_request),
                         "POST /authorize HTTP/1.1\n"
                         "Host: %s\n"
                         "Content-Type: application/json\n"
                         "Content-Length: %zu\n\n"
                         "{\"token\": \"%s\"}",
                         auth_server_ip, strlen(auth_token) + 13, auth_token);
  if (written < 0 || written > BUFFER_SIZE) {
    perror("Failed to format auth request");
    return -1;
  }
  char auth_response[BUFFER_SIZE];
  if (send_http_request(auth_sock, auth_request, written, auth_response,
                        BUFFER_SIZE) < 0) {
    close(auth_sock);
    return 1;
  }

  close(auth_sock);

  char token[256];
  parse_auth_response(auth_response, token);

  // fan_out
  log_node *log_root = NULL;
  char log_request[SERVER_NUMBER][BUFFER_SIZE];
  int request_length[SERVER_NUMBER];
  char log_response[SERVER_NUMBER][BUFFER_SIZE];
  size_t response_read[SERVER_NUMBER];
  struct pollfd poll_fds[SERVER_NUMBER];

  int log_sockets[SERVER_NUMBER];
  // open sockets
  for (int server = 0; server < SERVER_NUMBER; server++) {
    log_sockets[server] = connect_to_server(auth_server_ip, AUTH_SERVER_PORT);
    if (log_sockets[server] < 0) {
      perror("Failed to open log socket");
      return -1;
    }
  }
  // format requests
  for (int server = 0; server < SERVER_NUMBER; server++) {
    request_length[server] =
        snprintf(log_request[server], sizeof(log_request[server]),
                 "GET /logs/%d HTTP/1.1\n"
                 "Host: %s\n"
                 "Authorization: Bearer %s\n\n",
                 server, auth_server_ip, token);
    if (request_length < 0) {
      perror("Could not format server log request");
      return -1;
    }
  }
  // issue all requests
  for (int server = 0; server < SERVER_NUMBER; server++) {
    if (linux_send(log_sockets[server], log_request[server],
                   request_length[server], 0) < 0) {
      perror("Send failed");
      return -1;
    }
    response_read[server] = 0;
    poll_fds[server].fd = log_sockets[server];
    poll_fds[server].events = POLLIN;
    poll_fds[server].revents = 0;
  }
  unsigned int received_responses = 0;
  while (received_responses < SERVER_NUMBER) {
    int poll_result = linux_ppoll(poll_fds, SERVER_NUMBER);
    if (poll_result < 0) {
      perror("Polling failed");
      return -1;
    }
    // check all the file descriptors for arrived data
    for (int server = 0; server < SERVER_NUMBER; server++) {
      if (poll_fds[server].fd > 0 &&
          (poll_fds[server].revents & (POLLIN | POLLHUP))) {
        received_responses++;
        // disable further polling on this socket
        poll_fds[server].fd = -1;
        int new_data = linux_recv(log_sockets[server],
                                  log_response[server] + response_read[server],
                                  BUFFER_SIZE - response_read[server] - 1, 0);
        if (new_data < 0) {
          perror("Failed to read from socket");
          return -1;
        }
        response_read[server] += new_data;
        log_response[server][new_data] = '\0';
        parse_log_events(log_response[server], &log_root);
      }
    }
  }
  // close sockets
  for (int server = 0; server < SERVER_NUMBER; server++) {
    close(log_sockets[server]);
  }

  // render
  render_logs_to_html(log_root);

  // Free log nodes
  log_node *current = log_root;
  while (current) {
    log_node *next = current->next;
    free(current);
    current = next;
  }

  return 0;
}