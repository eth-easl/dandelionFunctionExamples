#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "unistd.h"

const unsigned int SERVER_NUMBER = 10;

int main(int argc, char const *argv[]) {

  // read authorization server file
  FILE *log_server = fopen("/servers/server.txt", "r");
  if (log_server == NULL) {
    perror("Failed to open file to get log server\n");
    return -1;
  }
  char *server_line = NULL;
  size_t server_line_len = 0;
  if (__getline(&server_line, &server_line_len, log_server) < 0) {
    perror("Fauled to read line from auth server file\n");
    return -1;
  }

  // read auth response
  FILE *auth_file = fopen("/responses/auth", "r");
  if (auth_file == NULL) {
    perror("Failed to open auth file\n");
    return -1;
  }
  // get the json with the athorization
  char username[21] = {0};
  char token[257] = {0};
  if (fscanf(auth_file,
             "%*[^\"]\"authorized\":\"%20[^\"]\",\"token\":\"%256[^\"]\"}",
             username, token) < 0) {
    perror("Failed to parse line from token file");
    return -1;
  }

  for (unsigned int server_index = 0; server_index < SERVER_NUMBER;
       server_index++) {
    char request_path[100] = {0};
    if (snprintf(request_path, 100, "/requests/server_%d", server_index) < 0) {
      perror("Failed to format file path for log request");
      return -1;
    }
    // write authorization request
    FILE *log_request = fopen(request_path, "w+");
    fprintf(log_request, "GET http://%s/logs/%02d HTTP/1.1\n\n", server_line,
            server_index);
    fprintf(log_request, "{\"username\": \"%s\"}", username);
  }

  return 0;
}
