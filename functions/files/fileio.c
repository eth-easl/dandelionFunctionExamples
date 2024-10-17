#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "unistd.h"

/*
search file system for all folders beginning with "in"
for each such folder create a folder called "out" and recurse, copying files and
folders within the "in" folder that also start with "in" to a new one called
"out"
*/

char *new_path(char *parent, char *name) {
  size_t parent_len = strlen(parent);
  size_t name_len = strlen(name);
  char *path = calloc(parent_len + name_len + 2, 1);
  strcpy(path, parent);
  path[parent_len] = '/';
  strcpy(path + parent_len + 1, name);
  return path;
}

int copy_folder(char *in_parent, char *out_parent) {
  DIR *in_parent_stream = opendir(in_parent);
  if (in_parent_stream == NULL) {
    perror("Could not open directory as stream");
    return -1;
  }
  struct dirent *entry;
  while ((entry = readdir(in_parent_stream))) {
    if (entry->d_name[0] == 'i' && entry->d_name[1] == 'n') {
      // create new name
      char name_buf[256] = {'o', 'u', 't'};
      strncpy(name_buf + 3, entry->d_name + 2, 253);
      char *new_out = new_path(out_parent, name_buf);
      char *new_in = new_path(in_parent, entry->d_name);
      if (entry->d_type == DT_DIR) {
        // check if folder exists
        DIR* out_parent = opendir(new_out);
        if(out_parent == NULL && errno == ENOENT){
          if (mkdir(new_out, 0x777) < 0) {
            perror("Failed to create new directory");
            return -1;
          }
        } else if(out_parent == NULL) {
          perror("Failed check for existing directory");
          return -1;
        } else {
          closedir(out_parent);
        }

        if (copy_folder(new_in, new_out) < 0) {
          printf("Failed to copy in folder %s\n", entry->d_name);
          return -1;
        }
      } else if (entry->d_type == DT_REG) {
        FILE *in_file = fopen(new_in, "r");
        if (in_file == NULL) {
          perror("Failed to open in file");
          return -1;
        }
        FILE *out_file = fopen(new_out, "w+");
        if (out_file == NULL) {
          perror("Failed to open out file");
          return -1;
        }
        char buffer[128];
        size_t read_chars = 0;
        while ((read_chars = fread(buffer, 1, 128, in_file))) {
          fwrite(buffer, 1, read_chars, out_file);
        }
        fclose(in_file);
        fclose(out_file);
      }
    }
  }
  return 0;
}

int main(int argc, char const *argv[]) {
  if (copy_folder("/", "/") < 0) {
    printf("Failed to copy in root folder\n");
    return -1;
  }
  return 0;
}
