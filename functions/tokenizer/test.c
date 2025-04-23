// #include "dandelion/crt.h"
#include "dandelion/runtime.h"
#include "dandelion/system/system.h"

int main() {
    size_t input_set_count = dandelion_input_set_count();
    if (input_set_count != 0) return -1;
    // printf("%d", input_set_count);
    
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
