#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "unistd.h"

int main(int argc, char const *argv[]) {

  // read authorization server file
  FILE *auth_server = fopen("/servers/server.txt", "r");
  if (auth_server == NULL) {
    perror("Failed to open file to get auth server");
    return -1;
  }
  char *server_line = NULL;
  size_t server_line_len = 0;
  if (__getline(&server_line, &server_line_len, auth_server) < 0) {
    perror("Fauled to read line from auth server file\n");
    return -1;
  }

  // read token from incomming file
  FILE *token_file = fopen("/responses/Authorization", "r");
  if (token_file == NULL) {
    perror("Failed to open file with token");
    return -1;
  }
  char token[257] = {0};
  if (fscanf(token_file, "Bearer %256s", token) < 0) {
    perror("Failed to parse line from token file");
    return -1;
  }

  // write authorization request
  FILE *auth_request = fopen("/requests/auth", "w+");
  fprintf(auth_request, "POST http://%s/authorize HTTP/1.1\n", server_line);
  fprintf(auth_request, "Content-Type: application/json\n\n");
  fprintf(auth_request, "{\"token\": \"%s\"}", token);

  return 0;
}
