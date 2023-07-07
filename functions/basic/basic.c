#include "dandelion/runtime.h"

// most basic function that does nothing except for returning
void _start(void) {
  dandelion_init();
  dandelion_exit(0);
}
