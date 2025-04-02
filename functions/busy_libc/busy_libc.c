#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "unistd.h"

#define ARRAY_ITEMS 16
#define FETCH_REQUEST_PATH "/data/get_request"
#define STORE_PREAMBLE_PATH "/preamble/post_request"
#define STORE_RESPONSE_PATH "/store_request/post_response"

typedef struct data_item {
    int8_t value[ARRAY_ITEMS];
} data_item;

size_t read_file_to_string(const char *filename, char **content) {
    // Open file in read mode
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    // Allocate memory for content (+1 for null terminator)
    *content = (char *)malloc(file_size + 1);
    if (*content == NULL) {
        perror("Error allocating memory");
    }

    // Read file into the buffer
    size_t bytes_read = fread(*content, 1, file_size, file);
    (*content)[bytes_read] = '\0'; // Add null terminator
    fclose(file);

    return bytes_read; // Return the length of the content
}

int main() {
    char *data = NULL;
    size_t data_len = read_file_to_string(FETCH_REQUEST_PATH, &data);
    if (data == NULL)
        return 1;

    uint64_t iterations = *(uint64_t *)data;
    data_item *data_items = (data_item *)(data + 8);

    // the igreggated statistics we want to collect
    // sum, average, variance, min, max
    int64_t sum = 0;
    int64_t min = 256;
    int64_t max = -256;

    size_t struct_index = 0;
    size_t value_index = 0;
    size_t max_index = (data_len - 8) / sizeof(data_item);
    for (size_t iteration = 0; iteration < iterations; iteration++) {
        for (size_t array_index = 0; array_index < ARRAY_ITEMS; array_index++) {
            int8_t value = data_items[struct_index].value[array_index];
            if (value > max)
                max = value;
            if (value < min)
                min = value;
            sum += value;
        }
        // using size_t which has no sign, guarantees that the index is not
        // negative when computing the remainder
        struct_index =
            (struct_index + data_items[struct_index].value[value_index]) %
            max_index;
        value_index = (value_index + 1) % ARRAY_ITEMS;
    }

    char *store_preamble = NULL;
    size_t store_preamble_len =
        read_file_to_string(STORE_PREAMBLE_PATH, &store_preamble);
    if (store_preamble == NULL)
        return 7;
    store_preamble_len -= 2; // omit \n\n at the end

    char header[] = "\nContent-Length: 24\n\n"; // 3 * sizeof(int64_t)
    size_t store_request_len =
        store_preamble_len + (sizeof(header) - 1) + 3 * sizeof(int64_t);
    char *store_request = malloc(store_request_len);
    char *current_ptr = store_request;
    memcpy(current_ptr, store_preamble, store_preamble_len);
    current_ptr += store_preamble_len;
    memcpy(current_ptr, header, sizeof(header) - 1);
    current_ptr += sizeof(header) - 1;
    memcpy(current_ptr, &sum, sizeof(int64_t));
    current_ptr += sizeof(int64_t);
    memcpy(current_ptr, &min, sizeof(int64_t));
    current_ptr += sizeof(int64_t);
    memcpy(current_ptr, &max, sizeof(int64_t));
    current_ptr += sizeof(int64_t);

    FILE *store_response_file = fopen(STORE_RESPONSE_PATH, "wb");
    if (store_response_file == NULL) {
        perror("Error opening file");
        return 9;
    }
    fwrite(store_request, 1, store_request_len, store_response_file);
    fclose(store_response_file);

    return 0;
}
