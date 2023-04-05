// System Headers
// #include <stdio.h>
#include <stdint.h>
// Standard Libraries

// Project External Libraries

// Project Internal Libraries
#include "dandelionIO.h"

// takes two inputs, an int with the number of rows/columns of a square matrix
// and a square matrix of int64s as well as a third input
// that is big enough to store the output (to avoid malloc for this simple test)
int main(void) {
  size_t inMatSize = getInputSize(1);
  int64_t* inMat = getInputPointer(1);
  size_t outMatSize = getInputSize(2);
  int64_t* outMat = getInputPointer(2);
  int matDim = inMatSize / sizeof(int64_t);

  addOutput();
  setOutputSize(0, outMatSize);
  setOutputPointer(0, outMat);

  for (int i = 0; i < matDim; i++) {
    for (int j = 0; j < matDim; j++) {
      for (int k = 0; k < matDim; k++) {
        outMat[i * matDim + j] += inMat[i * matDim + k] * inMat[j * matDim + k];
      }
    }
  }

  function_exit();
}
