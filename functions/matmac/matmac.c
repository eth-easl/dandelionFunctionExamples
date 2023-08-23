// System Headers
#include <stdint.h>
// Standard Libraries

// Project External Libraries

// Project Internal Libraries
#include "dandelion/runtime.h"

// compute a*b + c, if any is size zero replace by the identity element for the
// operation, the dimensions in the set for the matrix size give the rows and
// columns of the expeted output matrix.
int mat_mac() {
  // check we get expected inputs
  size_t input_set_count = dandelion_input_set_count();
  if (input_set_count != 3) return -1;
  size_t a_set_size = dandelion_input_buffer_count(0);
  size_t b_set_size = dandelion_input_buffer_count(1);
  size_t c_set_size = dandelion_input_buffer_count(2);

  int64_t* out_mat;
  int64_t mat_rows, mat_cols;

  // sanity check all available matrices for their size information
  for (int i = 0; i < 3; i++) {
    size_t set_size = dandelion_input_buffer_count(i);
    if (set_size == 1) {
      struct io_buffer* mat_buffer = dandelion_get_input(i, 0);
      size_t mat_size = mat_buffer->data_len;
      int64_t mat_rows = ((int64_t*)mat_buffer->data)[0];
      if (mat_size < 8 || mat_size % 8 != 0) return -2;
      mat_size = mat_size / 8 - 1;
      if (mat_rows > mat_size || mat_size % mat_rows != 0) return -3;
    }
  }

  if (c_set_size == 1) {
    struct io_buffer* c_mat_buffer = dandelion_get_input(2, 0);
    int64_t mat_size = (c_mat_buffer->data_len - 8) / 8;
    int64_t* c_mat = c_mat_buffer->data;
    mat_rows = c_mat[0];
    mat_cols = mat_size / mat_rows;
    out_mat = c_mat + 1;
  } else {
    if (a_set_size != 1 || b_set_size != 1) return -4;
    struct io_buffer* a_mat_buffer = dandelion_get_input(0, 0);
    struct io_buffer* b_mat_buffer = dandelion_get_input(1, 0);
    mat_rows = ((int64_t*)a_mat_buffer->data)[0];
    mat_cols =
        ((b_mat_buffer->data_len - 8) / 8) / ((int64_t*)b_mat_buffer->data)[0];
    out_mat = dandelion_alloc((mat_rows * mat_cols + 1) * 8, 8);
    out_mat++;
  }

  if (a_set_size == 1 && b_set_size == 1) {
    struct io_buffer* a_mat_buffer = dandelion_get_input(0, 0);
    struct io_buffer* b_mat_buffer = dandelion_get_input(1, 0);
    size_t common_length = ((a_mat_buffer->data_len - 1) / 8) / mat_rows;
    int64_t* a_mat = a_mat_buffer->data;
    a_mat++;
    int64_t* b_mat = b_mat_buffer->data;
    b_mat++;
    for (int64_t i = 0; i < mat_rows; i++) {
      for (int64_t j = 0; j < mat_cols; j++) {
        for (int64_t k = 0; k < common_length; k++) {
          out_mat[i * mat_cols + j] +=
              a_mat[i * common_length + k] * b_mat[k * mat_cols + j];
        }
      }
    }
  } else {
    int64_t* add_mat;
    if (a_set_size == 1 && b_set_size == 0) {
      struct io_buffer* a_mat_buffer = dandelion_get_input(0, 0);
      add_mat = a_mat_buffer->data;
    } else if (a_set_size == 0 && b_set_size == 1) {
      struct io_buffer* b_mat_buffer = dandelion_get_input(1, 0);
      add_mat = b_mat_buffer->data;
    } else
      return -5;
    add_mat++;
    for (int64_t i = 0; i < mat_rows * mat_cols; i++) {
      out_mat[i] += add_mat[i];
    }
  }

  struct io_buffer out_mat_buffer = {0};
  out_mat_buffer.data = --out_mat;
  out_mat[0] = mat_rows;
  out_mat_buffer.data_len = (mat_rows * mat_cols + 1) * 8;
  dandelion_add_output(0, out_mat_buffer);

  return 0;
}

// takes up to 4 inputs sets, first needs to contain 2 int64 with the number of
// rows and columns of the output matrix. Sets 1 to 3 contain a,b,c respectively
// for a*b+c, where one of them can be left out.
void _start(void) {
  dandelion_init();
  dandelion_exit(mat_mac());
}
