
/* --- include header files ------------------------------------------------ */

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

/* --- global variables ---------------------------------------------------- */

int FLAG_SQUARE;
int FLAG_CYLINDER;
#define HARMONICS 5

/* --- constants for Mersenne Twister pseudorandom number generator -------- */

// period parameters
#define N 624
#define M 397
#define MATRIX_A 0x9908b0df   // constant vector a
#define UPPER_MASK 0x80000000 // most significant w-r bits
#define LOWER_MASK 0x7fffffff // least significant r bits

// tempering parameters
#define TEMPERING_MASK_B 0x9d2c5680
#define TEMPERING_MASK_C 0xefc60000
#define TEMPERING_SHIFT_U(y) (y >> 11)
#define TEMPERING_SHIFT_S(y) (y << 7)
#define TEMPERING_SHIFT_T(y) (y << 15)
#define TEMPERING_SHIFT_L(y) (y >> 18)

/* --- define constants ---------------------------------------------------- */

#define BUZZ_SIZE 1024 // size of the buffer for name
#define EPS 1.0e-10 // precision value
#define MAX_SOURCE_SIZE (0x100000) // 16^5=1048576 number of lines of .cl file

/* --- functions prototypes ------------------------------------------------ */

cl_double length3(cl_double3 v);
double wtime(void);
int output_device_info(cl_device_id device_id);

// for Mersenne Twister

static unsigned long mt[N]; // the array for the state vector
static int mti=N+1; // mti==N+1 means mt[N] is not initialized

/* --- functions for Mersenne Twister -------------------------------------- */

// initializing the array with a seed
void sgenrand(unsigned long seed) {
  int i;
  for (i=0; i<N; i++) {
    mt[i] = seed & 0xffff0000;
    seed = 69069 * seed + 1;
    mt[i] |= (seed & 0xffff0000) >> 16;
    seed = 69069 * seed + 1;
  }
  mti = N;
}

/*
  Initialization by "sgenrand()" is an example. Theoretically, there are
  2^19937-1 possible states as an intial state. This function allows to
  choose any of 2^19937-1 ones. Essential bits in "seed_array[]" is
  following 19937 bits: (seed_array[0]&UPPER_MASK), seed_array[1], ...,
  seed_array[N-1]. (seed_array[0]&LOWER_MASK) is discarded. Theoretically,
  (seed_array[0]&UPPER_MASK), seed_array[1], ..., seed_array[N-1] can take
  any values except all zeros.
*/

void lsgenrand(unsigned long seed_array[]) {
  // the length of seed_array[] must be at least N
  int i;
  for (i=0; i<N; i++)
    mt[i] = seed_array[i];
  mti = N;
}

double genrand() {

  unsigned long y;
  static unsigned long mag01[2]={0x0, MATRIX_A};
  // mag01[x] = x * MATRIX_A  for x=0,1

  if (mti >= N) { /* generate N words at one time */
    int kk;

    if (mti == N+1) {  /* if sgenrand() has not been called, */
      sgenrand(4357); /* a default initial seed is used   */
    }

    for (kk=0; kk<N-M; kk++) {
      y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
      mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1];
    }

    for (; kk<N-1; kk++) {
      y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
      mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1];
    }

    y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
    mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1];

    mti = 0;
  }

  y = mt[mti++];
  y ^= TEMPERING_SHIFT_U(y);
  y ^= TEMPERING_SHIFT_S(y) & TEMPERING_MASK_B;
  y ^= TEMPERING_SHIFT_T(y) & TEMPERING_MASK_C;
  y ^= TEMPERING_SHIFT_L(y);

  return ( (double)y * 2.3283064365386963e-10 );
}

/* --- various functions --------------------------------------------------- */

double wtime(void) { // use a generic timer
  static int sec = -1;
  struct timeval tv;
  gettimeofday(&tv, NULL);
  if (sec < 0) sec = tv.tv_sec;
  return (tv.tv_sec - sec) + 1.0e-6*tv.tv_usec;
}

int output_device_info(cl_device_id device_id) {

  int err; // error code returned from OpenCL calls
  cl_device_type device_type; // parameter defining the type of the compute device
  cl_uint comp_units; // the max number of compute units on a device
  cl_char vendor_name[1024] = {0}; // string to hold vendor name for compute device
  cl_char device_name[1024] = {0}; // string to hold name of compute device
  cl_uint max_work_itm_dims;
  size_t max_wrkgrp_size;
  size_t *max_loc_size;

  err = clGetDeviceInfo(device_id, CL_DEVICE_NAME, sizeof(device_name), &device_name, NULL);
  if (err != CL_SUCCESS) {
    printf("error encountered while accessing device name\n");
    exit(EXIT_FAILURE);
  }
  printf("device: %s\n", device_name);

  err = clGetDeviceInfo(device_id, CL_DEVICE_TYPE, sizeof(device_type), &device_type, NULL);
  if (err != CL_SUCCESS) {
    printf("error encountered while accessing device type information\n");
    exit(EXIT_FAILURE);
  }

  if (device_type  == CL_DEVICE_TYPE_GPU) {
    printf("GPU from: ");
  } else if (device_type == CL_DEVICE_TYPE_CPU) {
    printf("CPU from: ");
  }

  err = clGetDeviceInfo(device_id, CL_DEVICE_VENDOR, sizeof(vendor_name), &vendor_name, NULL);
  if (err != CL_SUCCESS) {
    printf("error encountered while accessing device vendor name\n");
    exit(EXIT_FAILURE);
  }
  printf("%s\n", vendor_name);

  err = clGetDeviceInfo(device_id, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &comp_units, NULL);
  if (err != CL_SUCCESS) {
    printf("error encountered while accessing device number of compute units\n");
    exit(EXIT_FAILURE);
  }
  printf("max compute units: %d\n", comp_units);

  err = clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &max_work_itm_dims, NULL);
  if (err != CL_SUCCESS) {
      printf("error encountered while getting device info (CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS)\n");
      exit(EXIT_FAILURE);
  }

  max_loc_size = (size_t*)malloc(max_work_itm_dims * sizeof(size_t));
  if(max_loc_size == NULL) {
    printf("error encountered during memory allocation for max_loc_size\n");
    exit(EXIT_FAILURE);
  }

  err = clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_ITEM_SIZES, max_work_itm_dims* sizeof(size_t), max_loc_size, NULL);
  if (err != CL_SUCCESS) {
    printf("error encountered while getting device info (CL_DEVICE_MAX_WORK_ITEM_SIZES)\n");
    exit(EXIT_FAILURE);
  }

  err = clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &max_wrkgrp_size, NULL);
  if (err != CL_SUCCESS) {
    printf("error encountered while getting device info (CL_DEVICE_MAX_WORK_GROUP_SIZE)\n");
    exit(EXIT_FAILURE);
  }

  printf("max local dim: ");
  int i;
  for(i=0; i<max_work_itm_dims; i++) {
    printf(" %d", (int)(*(max_loc_size+i)));
  }
  printf("\n");
  printf("max work group size: %d\n", (int)max_wrkgrp_size);

  cl_uint max_constant_args;
  err = clGetDeviceInfo(device_id, CL_DEVICE_MAX_CONSTANT_ARGS, sizeof(cl_uint), &max_constant_args, NULL);
  if (err != CL_SUCCESS) {
    printf("error encountered while accessing device max constant args\n");
    exit(EXIT_FAILURE);
  }
  printf("DEVICE_MAX_CONSTANT_ARGS: %d\n", max_constant_args);
  return CL_SUCCESS;
}

cl_double length3(cl_double3 v) {
  cl_double longueur = sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
  return longueur;
}
