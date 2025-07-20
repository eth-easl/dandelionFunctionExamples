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
    char *message_state = NULL;
    char *llm_model = NULL;
    char *llm_endpoint = NULL;
    char *max_iters = NULL;
    size_t llm_reply_len = 0;
    size_t message_state_len = 0;
    size_t llm_model_len = 0;
    size_t llm_endpoint_len = 0;
    size_t max_iters_len = 0;

    get_input_item("/responses/llm_request", &llm_reply, &llm_reply_len);
    get_input_item("/message_state_in/message_state", &message_state, &message_state_len);
    get_input_item("/config/llm_model", &llm_model, &llm_model_len);
    get_input_item("/config/llm_endpoint", &llm_endpoint, &llm_endpoint_len);
    get_input_item("/config/max_iters", &max_iters, &max_iters_len);

    char* llm_reply_content = NULL;
    int err = extract_message_content(llm_reply, llm_reply_len, &llm_reply_content);
    if (err != SUCCESS) {
        return -1;
    }
    
    // LOOP END 2: LLM decided to stop
    if (strcmp(llm_reply_content, "\"STOP\"") == 0 || strcmp(llm_reply_content, "STOP") == 0) {
        printf("LLM decided to STOP. Exiting...\n");
        return -1;
    }

    // Update the message state with the assistant message
    cJSON *message_state_json = cJSON_ParseWithLength(message_state, message_state_len);
    cJSON *assistant_message = cJSON_CreateObject();
    cJSON_AddStringToObject(assistant_message, "role", "assistant");
    cJSON_AddStringToObject(assistant_message, "content", llm_reply_content);
    cJSON_AddItemToArray(message_state_json, assistant_message);

    // Write the request to execute a composition in BSON format
    // Start BSON document
    bson_t *composition_request_doc = bson_new();
    BSON_APPEND_UTF8(composition_request_doc, "name", "composition");

    // Start sets array
    bson_t sets;
    BSON_APPEND_ARRAY_BEGIN(composition_request_doc, "sets", &sets);

    // Start the first input set
    bson_t input_set;
    BSON_APPEND_DOCUMENT_BEGIN(&sets, "0", &input_set);
    BSON_APPEND_UTF8(&input_set, "identifier", "inputs");

    // Start the items array
    bson_t items;
    BSON_APPEND_ARRAY_BEGIN(&input_set, "items", &items);

    // Add llm_model item
    bson_t llm_model_item;
    BSON_APPEND_DOCUMENT_BEGIN(&items, "0", &llm_model_item);
    add_bson_item_data(llm_model, "llm_model", &llm_model_item);
    bson_append_document_end(&items, &llm_model_item);

    // Add llm_endpoint item
    bson_t llm_endpoint_item;
    BSON_APPEND_DOCUMENT_BEGIN(&items, "1", &llm_endpoint_item);
    add_bson_item_data(llm_endpoint, "llm_endpoint", &llm_endpoint_item);
    bson_append_document_end(&items, &llm_endpoint_item);

    // Add message state item
    bson_t message_state_item;
    BSON_APPEND_DOCUMENT_BEGIN(&items, "2", &message_state_item);
    add_bson_item_data(cJSON_PrintUnformatted(message_state_json), "message_state", &message_state_item);
    bson_append_document_end(&items, &message_state_item);

    // Add max_iters item
    bson_t max_iters_item;
    BSON_APPEND_DOCUMENT_BEGIN(&items, "3", &max_iters_item);
    add_bson_item_data(max_iters, "max_iters", &max_iters_item);
    bson_append_document_end(&items, &max_iters_item);

    bson_append_array_end(&input_set, &items);
    bson_append_document_end(&sets, &input_set);
    bson_append_array_end(composition_request_doc, &sets);

    // Invoke composition again
    const char * dandelion_server_composition_url = "http://127.0.0.1:8083/hot/c_test";
    write_http_request_from_bson("/requests/composition_request", dandelion_server_composition_url, composition_request_doc);
    
  return 0;
}
