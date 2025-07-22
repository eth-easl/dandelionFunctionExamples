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

    printf("function agent_3_process_composition_reply\n");
    
    char *composition_response;
    long composition_response_lem;
    // The composition reply is in BSON format, so need to read in binary.
    get_input_item_binary("/responses/composition_request", &composition_response, &composition_response_lem);

    // Parse composition reply as BSON
    bson_t *bson_doc = bson_new_from_data((const uint8_t *)composition_response, composition_response_lem);
    
    if (bson_doc) {
        printf("Start processing composition reply...\n");
        // Get the stdout output item
        const uint8_t *comp_stdout = NULL;
        uint32_t comp_stdout_len = 0;
        get_binary_data_item_from_bson("sets.0.items.1.data", bson_doc, &comp_stdout, &comp_stdout_len);
        printf("Printing stdout: %s\n", comp_stdout);

        // Get the stderr output item
        const uint8_t *comp_stderr = NULL;
        uint32_t comp_stderr_len = 0;
        get_binary_data_item_from_bson("sets.0.items.0.data", bson_doc, &comp_stderr, &comp_stderr_len);
        printf("Printing stderr: %s\n", comp_stderr);

        printf("End processing composition reply.\n");
        bson_destroy(bson_doc);
    } else {
        printf("Failed to parse composition reply as BSON.\n");
    }

    free(composition_response);
  return 0;
}
