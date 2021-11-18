#include "headers/square-cyl.h"

int main(int argc, char **argv) {

  printf("\nlpa-xrd version: !VERSION\n");

  if (argc != 7) {
    printf("%d arguments detected but 7 were expected.\n", argc);
    printf("Mode d'emploi : ./a.out <type> <block> <input> <nrp> <nfv> <output>\n");
    printf("type (int): hardware execution support (CPU:0 GPU:1)\n");
    printf("block (int): block size for the GPU block (from 2 to 256 depending on the GPU)\n");
    printf("input (string): path to the input data file\n");
    printf("nrp (int): number of random points for the Monte Carlo method\n");
    printf("nfv (int): number of values taken by the Fourier variable \n");
    printf("output (string): path and name of the output data file\n");
    exit(1);
  }

  printf("\nSTEP 01\n");
  #include "headers/read-parameters-from-file.h"

  printf("\nSTEP 02\n");
  #include "headers/replicated-dislocations.h"

  printf("\nSTEP 03\n");
  #include "headers/generate-random-points.h"

  printf("\nSTEP 04\n");
  #include "headers/define-opencl-accelerator.h"

  printf("\nSTEP 05\n");
  #include "headers/create-kernel1.h"

  printf("\nSTEP 06\n");
  #include "headers/create-kernel2.h"

  printf("\nSTEP 07\n");
  #include "headers/results-kernel1.h"

  printf("\nSTEP 08\n");
  #include "headers/compute-fourier-coefficients-kernel2.h"

  printf("\nSTEP 09\n");
  #include "headers/compute-error-dump-results.h"

  printf("\nSTEP 10\n");
  #include "headers/release-ressources.h"

  printf("\nSTEP 11\n");
  #include "headers/compute-performance.h"
}
