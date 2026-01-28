#include "dandelion/runtime.h"

char output = 0;

// most basic function that does nothing except for returning a simple fixed
// item
void _start(void) {
  dandelion_init();
  IoBuffer new_out = {.data = &output, .data_len = 1, .ident_len = 0, .key = 0};
  dandelion_add_output(0, new_out);
  dandelion_exit(0);
}
