#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "unistd.h"

int main(int argc, char const *argv[]) {
  // parse input
  FILE *input_data = fopen("/input/input.csv", "r");
  if (input_data == NULL) {
    perror("could not open /input/input.csv");
    return -1;
  }
  FILE *output_data = fopen("/output/output.csv", "w+");
  if (output_data == NULL) {
    perror("could not open /output/output.csv");
    return -1;
  }

  char *workload = NULL;
  size_t workload_size = 0;
  if (__getdelim(&workload, &workload_size, ',', input_data) < 0) {
    perror("failed to read workload");
    return -1;
  }
  workload[strlen(workload) - 1] = '\0';

  char *function = NULL;
  size_t function_size = 0;
  if (__getdelim(&function, &function_size, ',', input_data) < 0) {
    perror("failed to read function");
    return -1;
  }
  function[strlen(function) - 1] = '\0';

  char *requestedCpu = NULL;
  size_t requestedCpu_size = 0;
  if (__getdelim(&requestedCpu, &requestedCpu_size, ',', input_data) < 0) {
    perror("failed to read requestedCpu");
    return -1;
  }
  requestedCpu[strlen(requestedCpu) - 1] = '\0';

  char *multiplier = NULL;
  size_t multiplier_size = 0;
  if (__getdelim(&multiplier, &multiplier_size, ',', input_data) < 0) {
    perror("failed to read multiplier");
    return -1;
  }

  char *trace_string = "trace";
  char *empyt_string = "empty";

  if (strcmp(trace_string, function) == 0) {
    long multiplier_num = strtol(multiplier, NULL, 10);
    long requested_cpu_num = strtol(requestedCpu, NULL, 10);

    long total_iterations = multiplier_num * requested_cpu_num;

    // printf("%ld, %ld, %ld\n", multiplier_num, requested_cpu_num,
    // total_iterations);

    volatile double result = 0.0;
    volatile double input = 10.0;
    volatile long iteration;
    for (iteration = 0; iteration < total_iterations; iteration++) {
      result = sqrt(input);
    }

    fprintf(output_data, "\"OK\",\"%s\",\"dandelionServer\",%ld,%f", function,
            iteration, result);

    return 0;
  } else if (strcmp(empyt_string, function) == 0) {
    fprintf(output_data, "\"OK - EMPTY\",\"dandelionServer\",\"%s\",0",
            function);

    return 0;
  }

  return -1;
}
