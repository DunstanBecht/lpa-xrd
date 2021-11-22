// OpenCL platform interrogation

int err;

// get platform IDs

cl_uint nPlats;
err = clGetPlatformIDs(0, NULL, &nPlats);

cl_platform_id *platforms = (cl_platform_id *)malloc(sizeof(cl_platform_id) * nPlats);
err = clGetPlatformIDs(1, platforms, &nPlats);

printf("platforms IDs = %d\n", nPlats);

// load the kernel source code into the array source_str
FILE *fp;
char *source_str;
size_t source_size;
fp = fopen("kernels.cl", "r");
if (!fp) {
  printf("failed to load kernel\n");
  exit(1);
}
source_str = (char*)malloc(MAX_SOURCE_SIZE);
source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
fclose(fp);
printf("kernel code size = %d Bytes\n", (int)(source_size));

// get platform and device information
cl_platform_id platform_id = NULL;
cl_device_id device_id = NULL;
cl_uint ret_num_devices;
cl_uint ret_num_platforms;
cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);

// connect to a GPU (gpu=1) or a CPU (gpu=0)
int gpu = atoi(argv[1]);
printf("hardware: %d\n", gpu);
err = clGetDeviceIDs(platforms[0], gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
free(platforms);
platforms = NULL;

err = output_device_info(device_id);

size_t local_size;
size_t local_size_size;
clGetDeviceInfo(device_id, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(local_size), &local_size, &local_size_size);
printf("CL_DEVICE_LOCAL_MEM_SIZE: %d\n", (int)local_size);

// create an OpenCL context
cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

// create a command queue
cl_command_queue queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);

// create a program from the kernel source
cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, NULL, &ret);

// build the program executable
cl_int ret_val = clBuildProgram(program, 0, NULL,  "-cl-strict-aliasing -cl-unsafe-math-optimizations -cl-fast-relaxed-math", NULL, NULL);
if (ret_val==CL_SUCCESS) {
  printf("OpenCL code successfully compiled\n");
} else {
  char Buffer[16384];
  clGetProgramBuildInfo(program,device_id, CL_PROGRAM_BUILD_LOG, sizeof(Buffer), Buffer, NULL);
  fprintf(stderr, "OPenCL compilation error:\n%s", Buffer);
  exit(23);
}