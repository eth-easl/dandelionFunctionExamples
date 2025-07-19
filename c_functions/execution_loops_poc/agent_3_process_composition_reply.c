#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "cJSON.h"
#include <bson/bson.h>

#include "unistd.h"

int main(int argc, char const *argv[]) {

    printf("agent_3\n");

    // Load composition reply.
    FILE *compositon_reply_file = fopen("/responses/composition_request", "r");
    if (compositon_reply_file == NULL) {
        perror("Failed to open file with composition_request\n");
        return -1;
    }

    // The composition reply is in BSON format, so need to read in binary. (getline breaks because it is not UTF-8)
    // Get composition reply file size
    fseek(compositon_reply_file, 0, SEEK_END);
    long file_size = ftell(compositon_reply_file);
    rewind(compositon_reply_file);

    // Read the file
    char *buf = malloc(file_size);
    if (!buf) {
        perror("Failed to allocate buffer for composition reply");
        fclose(compositon_reply_file);
        return -1;
    }
    fread(buf, 1, file_size, compositon_reply_file);
    fclose(compositon_reply_file);

    printf("Printing printable chars from reply:\n");
    for (size_t i = 0; i < file_size; i++) {
        if (buf[i] >= 32 && buf[i] <= 126) {
            putchar(buf[i]);
        } else {
            printf("\\x%02x", (unsigned char)buf[i]);
        }
    }
    printf("\n");

    // Parse BSON
    const uint8_t *bson_buf = (const uint8_t *)buf;
    bson_t *doc = bson_new_from_data(bson_buf, file_size);

    if (doc) {
        // How to access the data field as readable text? BSON encodes it to base64.
        char *json_str = bson_as_canonical_extended_json(doc, NULL);
        printf("Parsed BSON as JSON:\n%s\n", json_str);
        bson_free(json_str);
        bson_destroy(doc);
    } else {
        printf("Failed to parse as BSON.\n");
    }

    free(buf);
  return 0;
}
