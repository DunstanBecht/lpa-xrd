/* may 2021 */
/* single version of the program: handles both cylindrical and square based geometry */
/* uses a moidified version of the input file  with keyword square or cylindrical */
/* square-cyl-ocl-mt-double-alldislo.c */
/* version du 3 septembre 2019 */
/*
 * utilise la memoire globale pour stocker les
 * positions des dislocations situees dans le plan equatorial
 * les positions des points aleatoires situes dans la sphere
*/
/* double
------------------------
Program Cylinder
November 2018
opencl version of the algorithm 
------------------------
*/
/*
------------------------
DEFINITION OF HEADER FILES 
------------------------
*/
#include<stdlib.h>
#include<stdio.h>
#include<sys/time.h>
#include<math.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
/*
------------------------
DEFINITION OF IMPORTANTS GLOBAL VARIABLES
------------------------
*/
int FLAG_SQUARE=0;
int FLAG_CYLINDER=1;
int D_REPLICATION=0; 
/* The number of replications ranges from -D to D in each direction */
/* a total of (2D+1)*(2D+1) cells is then constructed*/


/*
------------------------
CONSTANTS FOR MERSENNE TWISTER
------------------------
*/
/* Period parameters */  
#define N 624
#define M 397
#define MATRIX_A 0x9908b0df   /* constant vector a */
#define UPPER_MASK 0x80000000 /* most significant w-r bits */
#define LOWER_MASK 0x7fffffff /* least significant r bits */

/* Tempering parameters */   
#define TEMPERING_MASK_B 0x9d2c5680
#define TEMPERING_MASK_C 0xefc60000
#define TEMPERING_SHIFT_U(y)  (y >> 11)
#define TEMPERING_SHIFT_S(y)  (y << 7)
#define TEMPERING_SHIFT_T(y)  (y << 15)
#define TEMPERING_SHIFT_L(y)  (y >> 18)

/*
------------------------
DEFINITION OF CONSTANTS 
------------------------
*/
/* Size of the buffer for name */
#define BUZZ_SIZE 1024
/* Precision value */
#define EPS       1.0e-10
/* 16^5=1048576 Number of lines of .cl file */
#define MAX_SOURCE_SIZE (0x100000)
 
/*
------------------------
PROTOTYPES OF FUNCTIONS 
------------------------
*/
cl_double length3(cl_double3 v);
double wtime(void);
int output_device_info(cl_device_id device_id);

/*
------------------------
PROTOTYPES OF FUNCTIONS FOR MERSENNE TWISTER
------------------------
*/

static unsigned long mt[N]; /* the array for the state vector  */
static int mti=N+1; /* mti==N+1 means mt[N] is not initialized */

/*
------------------------
C CODE FOR MERSENNE TWISTER
------------------------
*/
/* Initializing the array with a seed */
void
sgenrand(seed)
    unsigned long seed;	
{
    int i;

    for (i=0;i<N;i++) {
         mt[i] = seed & 0xffff0000;
         seed = 69069 * seed + 1;
         mt[i] |= (seed & 0xffff0000) >> 16;
         seed = 69069 * seed + 1;
    }
    mti = N;
}

/* Initialization by "sgenrand()" is an example. Theoretically,      */
/* there are 2^19937-1 possible states as an intial state.           */
/* This function allows to choose any of 2^19937-1 ones.             */
/* Essential bits in "seed_array[]" is following 19937 bits:         */
/*  (seed_array[0]&UPPER_MASK), seed_array[1], ..., seed_array[N-1]. */
/* (seed_array[0]&LOWER_MASK) is discarded.                          */ 
/* Theoretically,                                                    */
/*  (seed_array[0]&UPPER_MASK), seed_array[1], ..., seed_array[N-1]  */
/* can take any values except all zeros.                             */
void
lsgenrand(seed_array)
    unsigned long seed_array[];
    /* the length of seed_array[] must be at least N */
{
    int i;

    for (i=0;i<N;i++) 
      mt[i] = seed_array[i];
    mti=N;
}

double /* generating reals */
/* unsigned long */ /* for integer generation */
genrand()
{
    unsigned long y;
    static unsigned long mag01[2]={0x0, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (mti >= N) { /* generate N words at one time */
        int kk;

        if (mti == N+1)   /* if sgenrand() has not been called, */
            sgenrand(4357); /* a default initial seed is used   */

        for (kk=0;kk<N-M;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1];
        }
        for (;kk<N-1;kk++) {
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

    return ( (double)y * 2.3283064365386963e-10 ); /* reals: [0,1)-interval */
    /* return y; */ /* for integer generation */
}

/*
------------------------
C CODE FOR VARIOUS FUNCTIONS
------------------------
*/
double wtime(void)
{
   /* Use a generic timer */
   static int sec = -1;
   struct timeval tv;
   gettimeofday(&tv, NULL);
   if (sec < 0) sec = tv.tv_sec;
   return (tv.tv_sec - sec) + 1.0e-6*tv.tv_usec;
}

int output_device_info(cl_device_id device_id)
{
    int err;                            // error code returned from OpenCL calls
    cl_device_type device_type;         // Parameter defining the type of the compute device
    cl_uint comp_units;                 // the max number of compute units on a device
    cl_char vendor_name[1024] = {0};    // string to hold vendor name for compute device
    cl_char device_name[1024] = {0};    // string to hold name of compute device
    cl_uint          max_work_itm_dims;
    size_t           max_wrkgrp_size;
    size_t          *max_loc_size;


    err = clGetDeviceInfo(device_id, CL_DEVICE_NAME, sizeof(device_name), &device_name, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to access device name!\n");
        return EXIT_FAILURE;
    }
    printf(" \n Device is  %s ",device_name);

    err = clGetDeviceInfo(device_id, CL_DEVICE_TYPE, sizeof(device_type), &device_type, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to access device type information!\n");
        return EXIT_FAILURE;
    }
    if(device_type  == CL_DEVICE_TYPE_GPU)
       printf(" GPU from ");

    else if (device_type == CL_DEVICE_TYPE_CPU)
       printf("\n CPU from ");

    else 
       printf("\n non  CPU or GPU processor from ");


    err = clGetDeviceInfo(device_id, CL_DEVICE_VENDOR, sizeof(vendor_name), &vendor_name, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to access device vendor name!\n");
        return EXIT_FAILURE;
    }
    printf(" %s ",vendor_name);

    err = clGetDeviceInfo(device_id, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &comp_units, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to access device number of compute units !\n");
        return EXIT_FAILURE;
    }
    printf(" with a max of %d compute units \n",comp_units);

    err = clGetDeviceInfo( device_id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), 
                               &max_work_itm_dims, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to get device Info (CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS)!\n");
        return EXIT_FAILURE;
    }
    
    max_loc_size = (size_t*)malloc(max_work_itm_dims * sizeof(size_t));
    if(max_loc_size == NULL){
       printf(" malloc failed\n");
       return EXIT_FAILURE;
    }
    err = clGetDeviceInfo( device_id, CL_DEVICE_MAX_WORK_ITEM_SIZES, max_work_itm_dims* sizeof(size_t), 
                               max_loc_size, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to get device Info (CL_DEVICE_MAX_WORK_ITEM_SIZES)!\n");
        return EXIT_FAILURE;
    }
    err = clGetDeviceInfo( device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), 
                               &max_wrkgrp_size, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to get device Info (CL_DEVICE_MAX_WORK_GROUP_SIZE)!\n");
        return EXIT_FAILURE;
    }
    printf("work group, work item information");
    printf("\n max loc dim ");
    int i;
    for(i=0; i< max_work_itm_dims; i++)
      printf(" %d ",(int)(*(max_loc_size+i)));
    printf("\n");
    printf(" Max work group size = %d\n",(int)max_wrkgrp_size);

    cl_uint max_constant_args;
    err = clGetDeviceInfo(device_id, CL_DEVICE_MAX_CONSTANT_ARGS, sizeof(cl_uint), &max_constant_args, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to access device max constant args!\n");
        return EXIT_FAILURE;
    }
    printf(" \n DEVICE_MAX_CONSTANT_ARGS is :  %d\n ",max_constant_args);
    return CL_SUCCESS;
}
/*
-- compute the norm of double3 vector
*/
cl_double length3(cl_double3 v)
{
  return sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
}

