// System Headers
#include <stdint.h>
// Standard Libraries

// Project External Libraries

// Project Internal Libraries
#include "dandelion/runtime.h"

int mat_mul() {
  // check we get expected inputs
  size_t input_set_count = dandelion_input_set_count();
  if (input_set_count < 2) return -1;
  size_t size_set_size = dandelion_input_buffer_count(0);
  if (size_set_size != 1) return -2;
  size_t mat_set_size = dandelion_input_buffer_count(1);
  if (mat_set_size != 1) return -3;

  struct io_buffer* size_buffer = dandelion_get_input(0, 0);
  struct io_buffer* in_mat_buffer = dandelion_get_input(1, 0);
  if (size_buffer->data_len != 8) return -4;
  size_t mat_size = *(size_t*)size_buffer->data;
  if (in_mat_buffer->data_len != mat_size * mat_size * 8) return -5;

  int64_t* in_mat = in_mat_buffer->data;
  int64_t* out_mat = dandelion_alloc(in_mat_buffer->data_len, 8);

  for (int i = 0; i < mat_size; i++) {
    for (int j = 0; j < mat_size; j++) {
      for (int k = 0; k < mat_size; k++) {
        out_mat[i * mat_size + j] +=
            in_mat[i * mat_size + k] * in_mat[j * mat_size + k];
      }
    }
  }

  struct io_buffer out_mat_buffer = {0};
  out_mat_buffer.data = out_mat;
  out_mat_buffer.data_len = in_mat_buffer->data_len;

  dandelion_add_output(0, out_mat_buffer);
  return 0;
}

// takes two inputs, an int with the number of rows/columns of a square matrix
// and a square matrix of int64s as well as a third input
// that is big enough to store the output (to avoid malloc for this simple
// test)
void _start(void) {
  dandelion_init();
  dandelion_exit(mat_mul());
}
