// System Headers
#include <stdint.h>
// Standard Libraries

// Project External Libraries

// Project Internal Libraries
#include "dandelion/runtime.h"
#include "dandelion/crt.h"

int mat_mul() {
  // check we get expected inputs
  size_t input_set_count = dandelion_input_set_count();
  if (input_set_count != 1) return -1;
  size_t mat_set_size = dandelion_input_buffer_count(0);
  if (mat_set_size != 1) return -2;

  struct io_buffer* mat_buffer = dandelion_get_input(0, 0);
  if (mat_buffer->data_len < 8) return -3;
  int64_t mat_rows = *(int64_t*)mat_buffer->data;
  size_t mat_size = mat_buffer->data_len - 8;
  if (mat_size < 8 || mat_size % 8 != 0 || (mat_size / 8) % mat_rows != 0)
    return -4;
  int64_t mat_cols = (mat_size / 8) / mat_rows;

  int64_t* in_mat = (int64_t*)mat_buffer->data + 1;
  int64_t* out_mat = dandelion_alloc(mat_rows * mat_rows * 8 + 8, 8);
  out_mat[0] = mat_rows;
  out_mat++;

  for (int i = 0; i < mat_rows; i++) {
    for (int j = 0; j < mat_rows; j++) {
      for (int k = 0; k < mat_cols; k++) {
        out_mat[i * mat_rows + j] +=
            in_mat[i * mat_cols + k] * in_mat[j * mat_cols + k];
      }
    }
  }

  struct io_buffer out_mat_buffer = {0};
  out_mat_buffer.data = out_mat - 1;
  out_mat_buffer.key = mat_buffer->key;
  out_mat_buffer.data_len = mat_rows * mat_rows * 8 + 8;

  dandelion_add_output(0, out_mat_buffer);
  return 0;
}

// takes two inputs, an int with the number of rows/columns of a square matrix
// and a square matrix of int64s as well as a third input
// that is big enough to store the output (to avoid malloc for this simple
// test)
DANDELION_ENTRY(mat_mul)
