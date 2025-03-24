// System Headers
#include "dandelion/crt.h"
#include "dandelion/runtime.h"
#include "dandelion/system/system.h"
#include <stdint.h>
// Standard Libraries

// Project External Libraries
#define QOI_NO_STDIO
#define QOI_MALLOC(size) dandelion_alloc(size, 8)
#define QOI_FREE(ptr) dandelion_free(ptr)
#define QOI_IMPLEMENTATION
#include "qoi.h"

void *realloc_sized(void *old, size_t old_size, size_t new_size) {
  // if new size is smaller just pretend we removed the bigger part
  if (new_size < old_size) {
    return old;
  }
  void *new = dandelion_alloc(new_size, 8);
  if (new == NULL) {
    return NULL;
  }
  memcpy(new, old, old_size);
  //   dandelion_free(old);
  return new;
}

#define STBI_WRITE_NO_STDIO
#define STBIW_MALLOC(size) dandelion_alloc(size, 8)
#define STBIW_FREE(ptr) dandelion_free(ptr)
#define STBIW_REALLOC_SIZED(ptr, old_size, new_size)                           \
  realloc_sized(ptr, old_size, new_size)
#define PNG_ONLY
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main() {
  // check we get expected inputs
  size_t input_set_count = dandelion_input_set_count();
  if (input_set_count != 1)
    return -1;
  size_t qoi_set_size = dandelion_input_buffer_count(0);
  if (qoi_set_size != 1)
    return -2;

  IoBuffer *qoi_buffer = dandelion_get_input(0, 0);

  qoi_desc in_descriptor = {};
  int qoi_size = qoi_buffer->data_len;
  void *pixels = qoi_decode(qoi_buffer->data, qoi_size, &in_descriptor, 0);

  if (pixels == NULL) {
    return -3;
  }

  int png_len;
  void *png = stbi_write_png_to_mem(pixels, 0, in_descriptor.width,
                                    in_descriptor.height,
                                    in_descriptor.channels, &png_len);
  if (png == NULL) {
    return -4;
  }

  IoBuffer png_buffer = {0};
  png_buffer.data = png;
  png_buffer.key = qoi_buffer->key;
  png_buffer.data_len = png_len;

  dandelion_add_output(0, png_buffer);
  return 0;
}

DANDELION_ENTRY(main);