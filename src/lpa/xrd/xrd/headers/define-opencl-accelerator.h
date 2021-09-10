  /*
  -------------------------
  OpenCL platform interrogation
  -------------------------
  */

  int err;
  // Get platform IDs
  cl_uint nPlats;
  err  = clGetPlatformIDs( 0, NULL, &nPlats);
  cl_platform_id *platforms = (cl_platform_id *)malloc(sizeof(cl_platform_id) * nPlats);
  err  = clGetPlatformIDs( 1, platforms, &nPlats);
  printf("Num platforms IDs = %d\n",nPlats);


  // Load the kernel source code into the array source_str
  FILE *fp;
  char *source_str;
  size_t source_size;
  fp = fopen("dpcaltfalldislo.cl", "r");
  if (!fp) 
  {
    fprintf(stderr, "Failed to load kernel.\n");
    exit(1);
  }
  source_str = (char*)malloc(MAX_SOURCE_SIZE);
  source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
  fclose( fp );
  printf("Kernel size       = %d Bytes\n",(int)(source_size));

  // Get platform and device information
  cl_platform_id platform_id = NULL;
  cl_device_id device_id = NULL;
  cl_uint ret_num_devices;
  cl_uint ret_num_platforms;
  cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);

  /* choose the kind of accelerator on which the kernel is executed */
  // Connect to a GPU (gpu=1) or a CPU (gpu=0)
  int gpu = atoi(argv[1]);
  printf("type accelerateur= %d\n   : 1=gpu 0=cpu",gpu);
  err = clGetDeviceIDs(platforms[0], gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
  free(platforms);
  platforms = NULL;

  // print context
  err = output_device_info(device_id);


  /*-- --*/
  size_t local_size;
  size_t local_size_size;
  clGetDeviceInfo(device_id, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(local_size), &local_size, &local_size_size);
  printf("CL_DEVICE_LOCAL_MEM_SIZE = %d\n", (int)local_size);

  // Create an OpenCL context
  cl_context context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);

  // Create a command queue
  cl_command_queue queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);

  /* Create a program from the kernel source */
  cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, NULL, &ret);

  // Build the program executable
  cl_int ret_val=clBuildProgram(program, 0, NULL,  "-cl-strict-aliasing -cl-unsafe-math-optimizations -cl-fast-relaxed-math", NULL, NULL);
  if ( ret_val==CL_SUCCESS)
       printf("Compilation du programme ok\n");
  else
  {
      char Buffer[16384];
      clGetProgramBuildInfo(program,device_id,CL_PROGRAM_BUILD_LOG,sizeof(Buffer),Buffer, NULL);
      fprintf(stderr,"CL Compilation unsuccessful:\n%s",Buffer);
      exit(23);
  }
