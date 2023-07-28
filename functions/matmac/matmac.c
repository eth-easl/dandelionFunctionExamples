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
  if (input_set_count > 4) return -1;
  size_t size_set_size = dandelion_input_buffer_count(0);
  if (size_set_size != 2) return -2;
  size_t a_set_size = dandelion_input_buffer_count(1);
  size_t b_set_size = dandelion_input_buffer_count(2);
  size_t c_set_size = dandelion_input_buffer_count(3);

  struct io_buffer* row_buffer = dandelion_get_input(0, 0);
  if (row_buffer->data_len != 8) return -4;
  size_t mat_rows = *(int64_t*)row_buffer->data;
  struct io_buffer* col_buffer = dandelion_get_input(0, 1);
  if (col_buffer->data_len != 8) return -5;
  size_t mat_cols = *(int64_t*)col_buffer->data;

  int64_t* out_mat;
  if (c_set_size == 1) {
    struct io_buffer* c_mat_buffer = dandelion_get_input(3, 0);
    if (c_mat_buffer->data_len != mat_rows * mat_cols * 8) return -6;
    out_mat = (int64_t*)c_mat_buffer->data;
  } else {
    out_mat = dandelion_alloc(mat_rows * mat_cols * 8, 8);
  }

  if (a_set_size == 1 && b_set_size == 1) {
    struct io_buffer* a_mat_buffer = dandelion_get_input(1, 0);
    struct io_buffer* b_mat_buffer = dandelion_get_input(2, 0);
    if (a_mat_buffer->data_len % 8 != 0 ||
        (a_mat_buffer->data_len / 8) % mat_rows != 0)
      return -7;
    size_t common_length = (a_mat_buffer->data_len / 8) / mat_rows;
    if (b_mat_buffer->data_len % 8 != 0 ||
        (b_mat_buffer->data_len / 8) % mat_cols != 0 ||
        (b_mat_buffer->data_len / 8) % common_length != 0)
      return -8;
    int64_t* a_mat = a_mat_buffer->data;
    int64_t* b_mat = b_mat_buffer->data;
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
      struct io_buffer* a_mat_buffer = dandelion_get_input(1, 0);
      if (a_mat_buffer->data_len != mat_rows * mat_cols * 8) return -9;
      add_mat = a_mat_buffer->data;
    } else if (a_set_size == 0 && b_set_size == 1) {
      struct io_buffer* b_mat_buffer = dandelion_get_input(2, 0);
      if (b_mat_buffer->data_len != mat_rows * mat_cols * 8) return -9;
      add_mat = b_mat_buffer->data;
    } else
      return -10;
    for (int64_t i = 0; i < mat_rows * mat_cols; i++) {
      out_mat[i] += add_mat[i];
    }
  }

  int64_t* out_dims = dandelion_alloc(sizeof(int64_t) * 2, 8);
  out_dims[0] = mat_rows;
  out_dims[1] = mat_cols;
  struct io_buffer out_row_buffer = {0};
  out_row_buffer.data = &out_dims[0];
  out_row_buffer.data_len = sizeof(int64_t);
  dandelion_add_output(0, out_row_buffer);

  struct io_buffer out_col_buffer = {0};
  out_col_buffer.data = &out_dims[1];
  out_col_buffer.data_len = sizeof(int64_t);
  dandelion_add_output(0, out_col_buffer);

  struct io_buffer out_mat_buffer = {0};
  out_mat_buffer.data = out_mat;
  out_mat_buffer.data_len = mat_rows * mat_cols * 8;
  dandelion_add_output(1, out_mat_buffer);

  return 0;
}

// takes up to 4 inputs sets, first needs to contain 2 int64 with the number of
// rows and columns of the output matrix. Sets 1 to 3 contain a,b,c respectively
// for a*b+c, where one of them can be left out.
void _start(void) {
  dandelion_init();
  dandelion_exit(mat_mac());
}
