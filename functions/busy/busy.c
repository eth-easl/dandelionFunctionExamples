// System Headers
#include <stdint.h>
// Standard Libraries

// Project External Libraries

// Project Internal Libraries
#include "dandelion/crt.h"
#include "dandelion/runtime.h"

void* local_memcpy(void* dest, const void* src, size_t n) {
  // copy using size_t steps if available
  size_t pointer_size = sizeof(size_t);
  size_t copied = 0;
  size_t to_copy = pointer_size * (n / pointer_size);
  size_t* to = dest;
  const size_t* from = src;
  for (; copied < to_copy; copied += pointer_size) {
    *to = *from;
    from++;
    to++;
  }
  uint8_t* to8 = (uint8_t*)to;
  const uint8_t* from8 = (uint8_t*)from;
  for (; copied < n; copied++) {
    *to8 = *from8;
    from8++;
    to8++;
  }
  return dest;
}
int busy() {
  // we expect two sets, one with the response data we work on
  // another one with the the response preable to attach the body to 
  size_t input_set_count = dandelion_input_set_count();
  if (input_set_count != 2) return -1;
  size_t data_set_size = dandelion_input_buffer_count(0);
  if (data_set_size != 1) return -2;
  size_t preamble_set_size = dandelion_input_buffer_count(1);
  if (preamble_set_size != 1) return -3;

  struct io_buffer* data_buffer = dandelion_get_input(0, 0);
  struct io_buffer* preamble_buffer = dandelion_get_input(1, 0);

  // start reading the http response that is the input
  char* data = (char*) data_buffer->data;
  size_t data_len = data_buffer->data_len;
  size_t preamble_len = preamble_buffer->data_len;
  if (data_len < 8) return -4;
  if ((size_t) data % 8 != 0) return -5;

  uint64_t iterations = *(uint64_t*)data;
  
  size_t output_len = data_len + preamble_len;
  char* output_buffer = dandelion_alloc(output_len, 64);
  local_memcpy(output_buffer, preamble_buffer->data, preamble_len);

#if defined(__x86_64__)
  uint64_t counter = 0;
  __asm__ volatile(
      "1:\n"
      "addq $1, %[counter]\n"
      "cmp %[counter], %[iterations]\n"
      "jg 1b\n"
      : [counter] "+r"(counter)
      : [iterations] "r"(iterations));

#elif defined(__aarch64__)
  uint64_t counter = 0;
  __asm__ volatile(
      "1:\n"
      "add %[counter], %[counter], #1\n"
      "cmp %[iterations], %[counter]\n"
      "b.gt 1b\n"
      : [counter] "+r"(counter)
      : [iterations] "r"(iterations));
#elif defined(__wasm__)
  volatile uint64_t counter = 0;
  while (counter < iterations) {
    counter += 1;
  }
#else
#error "Unimplemented architecture"
#endif

  *((uint64_t*)(output_buffer + preamble_len)) = counter;

  // keep ident and key from data buffer
  struct io_buffer request_buffer = *data_buffer;
  request_buffer.data = output_buffer;
  request_buffer.data_len = output_len;

  dandelion_add_output(0, request_buffer);
  return 0;
}

// takes two inputs, first one is an blob of data to hand through the function
// that also contains a number that tells the function to loop a X times.
// The second input is a preamble to format the output into a http request with
// the previous data as body 
DANDELION_ENTRY(busy)
