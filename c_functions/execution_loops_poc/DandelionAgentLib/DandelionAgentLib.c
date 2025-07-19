#include <stdio.h>
#include "DandelionAgentLib.h"

void write_llm_request(const char *file_path, const char *llm_endpoint, const char *payload) {
    FILE *llm_request = fopen("/requests/llm_request", "w+");
    fprintf(llm_request, "POST %s HTTP/1.1\n", llm_endpoint);
    fprintf(llm_request, "Content-Type: application/json\n\n");
    fprintf(llm_request, "%s", payload);
}
