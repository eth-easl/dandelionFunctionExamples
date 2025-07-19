#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "cJSON.h"
#include <bson/bson.h>
#include "DandelionAgentLib.h"

#include "unistd.h"

int main(int argc, char const *argv[]) {

    printf("agent_3\n");
    char *composition_response;
    long input_len;
    get_input_item_bson("/responses/composition_request", &composition_response, &input_len);

    print_as_ascii(composition_response, input_len);

    // Parse BSON
    bson_t *bson_doc = bson_new_from_data((const uint8_t *)composition_response, input_len);

    if (bson_doc) {
        // How to access the data field as readable text? BSON encodes it to base64.
        char *json_str = bson_as_canonical_extended_json(bson_doc, NULL);
        printf("Parsed BSON as JSON:\n%s\n", json_str);
        bson_free(json_str);
        bson_destroy(bson_doc);
    } else {
        printf("Failed to parse as BSON.\n");
    }

    free(composition_response);
  return 0;
}
