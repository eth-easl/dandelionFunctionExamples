#include <errno.h>
#include <stdio.h>

#include "unistd.h"

int main(int argc, char const *argv[]) {
  // print to std out and std err with the usual mode
  int err;
  dprintf(STDOUT_FILENO, "Files: %p, %p, %p", stdin, stdout, stderr);
  // if ((err = printf("Test string to stdout\n")) < 0) {
  //   return -1;
  // }
  // if ((err = fputs("Test string to stderr\n", stderr)) < 0) {
  //   return -2;
  // }
  // // // read stdin
  // // char in_buffer[128];
  // // ssize_t read_chars = fread(in_buffer, 1, 128, stdin);
  // // if (read_chars < 0) {
  // //   perror("Trying to read stdin failed\n");
  // //   return -3;
  // // }
  // // printf("read %zi characters from stdin\n", read_chars);
  // // ssize_t written = fwrite(in_buffer, 1, read_chars, stdout);
  // // if (written != read_chars) return -4;
  return 0;
}
