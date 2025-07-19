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

    printf("agent_2\n");
    char *llm_reply = NULL;
    size_t llm_reply_len = 0;
    int err_llm_reply = get_input_item("/responses/llm_request", &llm_reply, &llm_reply_len);

    char* llm_reply_content = NULL;
    int err = extract_message_content(llm_reply, llm_reply_len, &llm_reply_content);
    if (err != SUCCESS) {
        return -1;
    }

    // Start BSON document
    bson_t *doc = bson_new();
    BSON_APPEND_UTF8(doc, "name", "composition");

    // Start sets array
    bson_t sets;
    BSON_APPEND_ARRAY_BEGIN(doc, "sets", &sets);

    // Start the first input set
    bson_t set_obj;
    BSON_APPEND_DOCUMENT_BEGIN(&sets, "0", &set_obj);
    BSON_APPEND_UTF8(&set_obj, "identifier", "inputs");

    // Start the items array
    bson_t items;
    BSON_APPEND_ARRAY_BEGIN(&set_obj, "items", &items);

    // Add llm_model item
    bson_t llm_model_item;
    BSON_APPEND_DOCUMENT_BEGIN(&items, "0", &llm_model_item);
    add_bson_item_data("meta-llama/Llama-3.2-3B-Instruct", "llm_model", &llm_model_item);
    bson_append_document_end(&items, &llm_model_item);

    // Add llm_endpoint item
    bson_t llm_endpoint_item;
    BSON_APPEND_DOCUMENT_BEGIN(&items, "1", &llm_endpoint_item);
    add_bson_item_data("http://localhost:8081/v1/chat/completions", "llm_endpoint", &llm_endpoint_item);
    bson_append_document_end(&items, &llm_endpoint_item);

    // Add message state item
    bson_t message_state_item;
    BSON_APPEND_DOCUMENT_BEGIN(&items, "2", &message_state_item);
    add_bson_item_data(llm_reply_content, "message_state", &message_state_item);
    bson_append_document_end(&items, &message_state_item);

    bson_append_array_end(&set_obj, &items);
    bson_append_document_end(&sets, &set_obj);
    bson_append_array_end(doc, &sets);

    // Invoke composition again
    write_bson_http_request("/requests/composition_request", "http://127.0.0.1:8083/hot/c_test", doc);
    
  return 0;
}
