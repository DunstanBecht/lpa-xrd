#include "headers/header.h"

int main(int argc, char **argv) {

  printf("lpa-xrd version: !VERSION\n");

  if (argc != 7) {
    printf("%d arguments detected but 7 were expected.\n", argc);
    printf("Example: './a.out <hardware> <block> <input> <nrp> <nfv> <output>' with:\n");
    printf("hardware (int): execution support (CPU:0 / GPU:1)\n");
    printf("block (int): block size for the GPU block (from 2 to 256 depending on the GPU)\n");
    printf("input (string): path to the input data file\n");
    printf("nrp (int): number of random points for the Monte Carlo method\n");
    printf("nfv (int): number of values taken by the Fourier variable \n");
    printf("output (string): path and name of the output data file\n");
    exit(EXIT_FAILURE);
  }

  printf("STEP 1: load input data file\n");
  #include "headers/load_input_data.h"

  printf("STEP 2: generate random points\n");
  #include "headers/generate_random_points.h"

  printf("STEP 3: define opencl accelerator\n");
  #include "headers/define_opencl_accelerator.h"

  printf("STEP 4: create kernel 1\n");
  #include "headers/create_kernel_1.h"

  printf("STEP 5: create kernel 2\n");
  #include "headers/create_kernel_2.h"

  printf("STEP 6: get kernel 1 results\n");
  #include "headers/results_kernel_1.h"

  printf("STEP 7: get kernel 2 results\n");
  #include "headers/results_kernel_2.h"

  printf("STEP 8: export output data file\n");
  #include "headers/export_output_data.h"

  printf("STEP 9: compute GPU performance\n");
  #include "headers/compute_performance.h"

  printf("program ended successfully");
  return EXIT_SUCCESS;
}
