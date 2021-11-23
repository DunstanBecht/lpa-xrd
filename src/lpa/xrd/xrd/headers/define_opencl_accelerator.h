int err; // OpenCL error code
cl_uint nPlats; // number of OpenCL platforms available
FILE *fp; // OpenCL code source file
char *source_str; // OpenCL code source
size_t source_size; // OpenCL code source size

/* --- obtain the list of platforms available ------------------------------ */

err = clGetPlatformIDs(0, NULL, &nPlats);

cl_platform_id *platforms = (cl_platform_id *)malloc(sizeof(cl_platform_id) * nPlats);

err = clGetPlatformIDs(1, platforms, &nPlats);

printf("number of OpenCL platforms available: %d\n", nPlats);

/* --- load the kernel source code ----------------------------------------- */

fp = fopen("kernels.cl", "r");
if (!fp) {
  printf("error encountered while opening the OpenCL code source file\n");
  exit(EXIT_FAILURE);
}
source_str = (char*)malloc(MAX_SOURCE_SIZE);
source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
fclose(fp);
printf("memory space for OpenCL code: %d bytes\n", (int)(source_size));

/* --- choose a device ----------------------------------------------------- */

cl_platform_id platform_id = NULL;
cl_device_id device_id = NULL;
cl_uint ret_num_devices;
cl_uint ret_num_platforms;
cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);

int gpu = atoi(argv[1]);
printf("hardware choosen: %d\n", gpu);
err = clGetDeviceIDs(platforms[0], gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
free(platforms);
platforms = NULL;

/* --- get device information ---------------------------------------------- */

err = output_device_info(device_id);

size_t local_size;
size_t local_size_size;
clGetDeviceInfo(device_id, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(local_size), &local_size, &local_size_size);
printf("CL_DEVICE_LOCAL_MEM_SIZE: %d\n", (int)local_size);

/* --- create an OpenCL context -------------------------------------------- */

cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

/* --- create a command queue ---------------------------------------------- */

cl_command_queue queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);

/* --- create a program object for the context and load the source code ---- */

cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, NULL, &ret);

/* --- compile and link a program executable from the program source ------- */

cl_int ret_val = clBuildProgram(program, 0, NULL,  "-cl-strict-aliasing -cl-unsafe-math-optimizations -cl-fast-relaxed-math", NULL, NULL);
if (ret_val == CL_SUCCESS) {
  printf("OpenCL code successfully compiled\n");
} else {
  char Buffer[16384];
  clGetProgramBuildInfo(program,device_id, CL_PROGRAM_BUILD_LOG, sizeof(Buffer), Buffer, NULL);
  fprintf(stderr, "OpenCL compilation error:\n%s", Buffer);
  exit(EXIT_FAILURE);
}
