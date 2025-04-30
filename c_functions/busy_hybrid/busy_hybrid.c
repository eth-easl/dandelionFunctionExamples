#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "syscall.h"
#include "unistd.h"

#define ARRAY_ITEMS 16
#define BUFFER_SIZE 1 * 1024 * 1024
#define FETCH_REQUEST_PATH "/data/get_request"
#define STORE_PREAMBLE_PATH "/preamble/post_request"
#define STORE_RESPONSE_PATH "/store_request/post_response"

typedef struct data_item {
  int8_t value[ARRAY_ITEMS];
} data_item;

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

int extract_ip_port(const char *http_request, char *ip, size_t ip_len,
                    uint16_t *port) {
  const char *prefix = "http://";
  const char *start = strstr(http_request, prefix);
  if (!start)
    return -1;

  start += strlen(prefix);
  const char *colon = strchr(start, ':');
  if (!colon)
    return -2;

  const char *slash = strchr(colon, '/');
  if (!slash)
    return -3;

  // Extract IP
  size_t ip_size = colon - start;
  if (ip_size >= ip_len)
    return -4;
  strncpy(ip, start, ip_size);
  ip[ip_size] = '\0';

  // Extract port
  char port_str[6] = {0};
  size_t port_size = slash - colon - 1;
  if (port_size >= sizeof(port_str))
    return -5;
  strncpy(port_str, colon + 1, port_size);
  *port = atoi(port_str);

  return 0;
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

ssize_t send_http_request(int sockfd, const char *request, size_t request_size,
                          char *response, size_t response_size) {
  if (linux_send(sockfd, request, request_size, 0) < 0) {
    perror("Error sending request");
    return -1;
  }

  // TODO: ensure the entire HTTP response has been received
  ssize_t total_received = 0, bytes;
  // while ((bytes = linux_recv(sockfd, response + total_received,
  // response_size - total_received - 1, 0)) > 0) {
  //     total_received += bytes;
  //     if (total_received >= response_size - 1) break;
  // }
  // blocks and waits forever if there is no next packate, could validate HTTP
  // sice on the packages to see if we need more, this works for now
  total_received = linux_recv(sockfd, response + total_received,
                              response_size - total_received - 1, 0);
  if (total_received < 0) {
    perror("Error receiving response");
    return -1;
  }
  response[total_received] = '\0';
  return total_received;
}

void parse_http_body(const char *response, size_t response_size,
                     char **response_body, size_t *response_body_size) {
  *response_body = strstr(response, "\r\n\r\n");
  if (*response_body == NULL)
    return;
  *response_body += 4;
  *response_body_size = response_size - (*response_body - response);
}

size_t read_file_to_string(const char *filename, char **content) {
  // Open file in read mode
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    perror("Error opening file");
  }

  // Get file size
  fseek(file, 0, SEEK_END);
  size_t file_size = ftell(file);
  rewind(file);

  // Allocate memory for content (+1 for null terminator)
  *content = (char *)malloc(file_size + 1);
  if (*content == NULL) {
    perror("Error allocating memory");
  }

  // Read file into the buffer
  size_t bytes_read = fread(*content, 1, file_size, file);
  (*content)[bytes_read] = '\0'; // Add null terminator
  fclose(file);

  return bytes_read; // Return the length of the content
}

int main() {
  char *fetch_preamble = NULL;
  size_t fetch_preamble_len =
      read_file_to_string(FETCH_REQUEST_PATH, &fetch_preamble);
  if (fetch_preamble == NULL)
    return 1;

  char server_ip[16];
  uint16_t server_port;
  if (extract_ip_port(fetch_preamble, server_ip, 16, &server_port) < 0)
    return 2;

  // fetching data
  int sock = connect_to_server(server_ip, server_port);
  if (sock < 0)
    return 3;

  size_t fetch_request_len = fetch_preamble_len + 4;
  char *fetch_request = malloc(fetch_request_len);
  memcpy(fetch_request, fetch_preamble, fetch_preamble_len);
  memcpy(fetch_request + fetch_preamble_len, "\r\n\r\n", 4);

  char *fetch_response = (char *)malloc(BUFFER_SIZE);
  if (fetch_response == NULL) {
    perror("Error allocating memory");
    return 4;
  }
  ssize_t fetch_response_len = send_http_request(
      sock, fetch_request, fetch_request_len, fetch_response, BUFFER_SIZE);
  if (fetch_response_len < 0)
    return 5;
  close(sock);

  char *fetch_response_body;
  size_t fetch_response_body_len;
  parse_http_body(fetch_response, fetch_response_len, &fetch_response_body,
                  &fetch_response_body_len);
  if (fetch_response_body == NULL)
    return 6;

  uint64_t iterations = *(uint64_t *)fetch_response_body;
  data_item *data_items = (data_item *)(fetch_response_body + 8);

  // the igreggated statistics we want to collect
  // sum, average, variance, min, max
  int64_t sum = 0;
  int64_t min = 256;
  int64_t max = -256;

  size_t struct_index = 0;
  size_t value_index = 0;
  size_t max_index = (fetch_response_body_len - 8) / sizeof(data_item);
  for (size_t iteration = 0; iteration < iterations; iteration++) {
    for (size_t array_index = 0; array_index < ARRAY_ITEMS; array_index++) {
      int8_t value = data_items[struct_index].value[array_index];
      if (value > max)
        max = value;
      if (value < min)
        min = value;
      sum += value;
    }
    // using size_t which has no sign, guarantees that the index is not
    // negative when computing the remainder
    struct_index =
        (struct_index + data_items[struct_index].value[value_index]) %
        max_index;
    value_index = (value_index + 1) % ARRAY_ITEMS;
  }

  char *store_preamble = NULL;
  size_t store_preamble_len =
      read_file_to_string(STORE_PREAMBLE_PATH, &store_preamble);
  if (store_preamble == NULL)
    return 7;
  store_preamble_len -= 2; // omit \n\n at the end

  char header[] = "\r\ncontent-length: 24\r\n\r\n"; // 3 * sizeof(int64_t)
  size_t store_request_len =
      store_preamble_len + (sizeof(header) - 1) + 3 * sizeof(int64_t);
  char *store_request = malloc(store_request_len);
  char *current_ptr = store_request;
  memcpy(current_ptr, store_preamble, store_preamble_len);
  current_ptr += store_preamble_len;
  memcpy(current_ptr, header, sizeof(header) - 1);
  current_ptr += sizeof(header) - 1;
  memcpy(current_ptr, &sum, sizeof(int64_t));
  current_ptr += sizeof(int64_t);
  memcpy(current_ptr, &min, sizeof(int64_t));
  current_ptr += sizeof(int64_t);
  memcpy(current_ptr, &max, sizeof(int64_t));
  current_ptr += sizeof(int64_t);

  // posting data
  char *store_response = fetch_response;
  sock = connect_to_server(server_ip, server_port);
  if (sock < 0)
    return 3;
  ssize_t store_response_len = send_http_request(
      sock, store_request, store_request_len, store_response, BUFFER_SIZE);
  if (store_response_len < 0)
    return 8;
  close(sock);

  FILE *store_response_file = fopen(STORE_RESPONSE_PATH, "wb");
  if (store_response_file == NULL) {
    perror("Error opening file");
    return 9;
  }
  fwrite(store_response, 1, store_response_len, store_response_file);
  fclose(store_response_file);

  return 0;
}
