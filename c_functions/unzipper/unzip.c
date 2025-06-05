#include <stdio.h>
#include <string.h>

// #include "dandelion/crt.h"
#include "dandelion/runtime.h"
// #include "dandelion/system/system.h"

#include "zip.h"

size_t get_number_files() {
    size_t input_set_count = dandelion_input_set_count();
    if (input_set_count != 1) {
        printf("Expected a single set.\n");
        return -1;
    }
    const char* set_name = dandelion_input_set_ident(0);
    printf("Found %zu input set: \"%s\".\n", input_set_count, set_name);

    size_t first_set_count = dandelion_input_buffer_count(0);
    printf("Found %zu files:\n", first_set_count);
    if (first_set_count == 0) {
        printf("Expected at least one \".zip\" file in the input set.\n");
        return -1;
    }

    return first_set_count;
}

size_t get_zip_buffers(size_t n_all_files, size_t buffer_indexes[]) {
    size_t count = 0;
    for (int i = 0; i < n_all_files; i++) {
        IoBuffer* buffer = dandelion_get_input(0, i);
        const char* name = buffer->ident;

        char dot_char = 46; // "." in ASCII
        char* dot = strchr(name, dot_char);
        if (dot && !strcmp(dot, ".zip")) {
            printf("%d) Found buffer \"%s\": added to zip files\n", i, name);
            buffer_indexes[count++] = i;
        } else {
            printf("%d) Found buffer \"%s\": skipping, NOT a zip file\n", i, name);
        }
    }
    return count;
}

int main() {
    size_t n_all_files = get_number_files();

    size_t buffer_indexes[n_all_files];
    size_t n_zip = get_zip_buffers(n_all_files, buffer_indexes);

    int err = 0;
    zip_t *z = zip_open("foo.zip", 0, &err);

    for (int i = 0; i < n_zip; i++) {
        printf("%zu\n", buffer_indexes[i]);
    }


    
    int data_len = 13;
    // char greetings[] = "Hello World!\0";
    /*void *data = (void*)greetings;
  
    IoBuffer output = {0};
    output.data = data;
    output.key = 0;
    output.data_len = data_len;
  
    dandelion_add_output(0, output);*/
    return 0;
}
  
// DANDELION_ENTRY(main);
