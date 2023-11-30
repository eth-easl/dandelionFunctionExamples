// System Headers
#include <stdint.h>
// Standard Libraries

// Project External Libraries

// Project Internal Libraries
#include "dandelion/runtime.h"
#include "dandelion/crt.h"

static uint64_t counter;

int busy() {
  // check we get expected inputs
  size_t input_set_count = dandelion_input_set_count();
  if (input_set_count != 1) return -1;
  size_t set_size = dandelion_input_buffer_count(0);
  if (set_size != 1) return -2;

  struct io_buffer* busy_buffer = dandelion_get_input(0, 0);
  if (busy_buffer->data_len < 8) return -3;
  uint64_t iterations = *(uint64_t*)busy_buffer->data;

  counter = 0;

  for (uint64_t iteration = 0; iteration < iterations; iteration++) {
    counter += ((iteration & 0x1) << 1);
  }
  *((uint64_t*)busy_buffer->data) = counter;

  struct io_buffer out_busy_buffer = *busy_buffer;

  dandelion_add_output(0, out_busy_buffer);
  return 0;
}

// takes two inputs, an int with the number of rows/columns of a square matrix
// and a square matrix of int64s as well as a third input
// that is big enough to store the output (to avoid malloc for this simple
// test)
DANDELION_ENTRY(busy)
