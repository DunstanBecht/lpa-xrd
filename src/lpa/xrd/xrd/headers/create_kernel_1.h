size_t localSize1[1]; // kernel 1 local size
size_t globalSize1[1]; // kernel 1 global size
cl_kernel kernel1; // kernel object

cl_double2 *h_r1; // random point positions
cl_double3 *h_u1; // displacement field at the random points

localSize1[0] = atoi(argv[2]);
globalSize1[0] = localSize1[0]*ceil(Np/localSize1[0]); // must be a multiple of locaSize1
printf("kernel 1 global size: %lu\n", globalSize1[0]);
printf("kernel 1 local size: %lu\n", localSize1[0]);

/* --- create a kernel object ---------------------------------------------- */

kernel1 = clCreateKernel(program, "udislo", &err);
if (err != CL_SUCCESS) {
  printf("error encountered during kernel creation\n");
  exit(EXIT_FAILURE);
}

/* --- create data structures on the host ---------------------------------- */

h_r1 = (cl_double2 *)malloc(sizeof(cl_double2)*Np);
if (h_r1 == NULL) {
  printf("error encountered during memory allocation for h_r1\n");
  exit(EXIT_FAILURE);
}
printf("memory space h_r1: %lu bytes\n", sizeof(h_r1));

h_u1 = (cl_double3 *)malloc(sizeof(cl_double3)*Np);
if (h_u1 == NULL) {
  printf("error encountered during memory allocation for h_u1\n");
  exit(EXIT_FAILURE);
}
printf("memory space h_u1: %lu bytes\n", sizeof(h_u1));

/* --- create buffer objects ----------------------------------------------- */

printf("create input and output device buffers for Kernel 1\n");

size_t bytes_dislocations = sizeof(cl_double3) * Nd;
size_t bytes_r1 = sizeof(cl_double2) * Np;
size_t bytes_u1 = sizeof(cl_double3) * Np;

cl_mem d_dislocations = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_ALLOC_HOST_PTR, bytes_dislocations, NULL, NULL);
cl_mem d_random1 = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_ALLOC_HOST_PTR, bytes_random, NULL, NULL);
cl_mem d_random2 = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_ALLOC_HOST_PTR, bytes_random, NULL, NULL);
cl_mem d_r1 = clCreateBuffer(context, CL_MEM_READ_WRITE|CL_MEM_ALLOC_HOST_PTR, bytes_r1, NULL, NULL);
cl_mem d_u1 = clCreateBuffer(context, CL_MEM_READ_WRITE|CL_MEM_ALLOC_HOST_PTR, bytes_u1, NULL, NULL);

/* --- write to buffer objects from host memory ---------------------------- */

err = clEnqueueWriteBuffer(queue, d_dislocations, CL_TRUE, 0, bytes_dislocations, dislocations, 0, NULL, NULL);
err |= clEnqueueWriteBuffer(queue, d_random1, CL_TRUE, 0, bytes_random, random1, 0, NULL, NULL);
err |= clEnqueueWriteBuffer(queue, d_random2, CL_TRUE, 0, bytes_random, random2, 0, NULL, NULL);
err |= clEnqueueWriteBuffer(queue, d_r1, CL_TRUE, 0, bytes_r1, h_r1, 0, NULL, NULL);
err |= clEnqueueWriteBuffer(queue, d_u1, CL_TRUE, 0, bytes_u1, h_u1, 0, NULL, NULL);

if (err != CL_SUCCESS) {
  printf("error encountered while writing to buffer objects from host memory\n");
  exit(EXIT_FAILURE);
}

/* --- set the value of the kernel arguments ------------------------------- */

size_t shared_size_kernel1;
shared_size_kernel1 = localSize1[0] * sizeof(cl_double3);
printf("shared_size_kernel1: %d bytes\n", shared_size_kernel1);

err = clSetKernelArg(kernel1, 0, sizeof(cl_mem), &d_random1);
err |= clSetKernelArg(kernel1, 1, sizeof(cl_mem), &d_random2);
err |= clSetKernelArg(kernel1, 2, sizeof(cl_mem), &d_dislocations);
err |= clSetKernelArg(kernel1, 3, sizeof(cl_mem), &d_r1);
err |= clSetKernelArg(kernel1, 4, sizeof(cl_mem), &d_u1);
err |= clSetKernelArg(kernel1, 5, sizeof(cl_double), &be_len);
err |= clSetKernelArg(kernel1, 6, sizeof(cl_double), &bs_len);
err |= clSetKernelArg(kernel1, 7, sizeof(cl_double), &size);
err |= clSetKernelArg(kernel1, 8, sizeof(cl_double), &nu);
err |= clSetKernelArg(kernel1, 9, sizeof(cl_int), &Np);
err |= clSetKernelArg(kernel1, 10, sizeof(cl_int), &Nd);
err |= clSetKernelArg(kernel1, 11, sizeof(cl_int), &FLAG_SQUARE);
err |= clSetKernelArg(kernel1, 12, shared_size_kernel1, NULL); // size of the used shared memory

if (err != CL_SUCCESS) {
  printf("error encountered while setting the value of the kernel arguments\n");
  exit(EXIT_FAILURE);
}
printf("kernel 1 arguments definition done\n");

/* --- information about the kernel that may be specific to the device ----- */

size_t thread_size;
clGetKernelWorkGroupInfo(kernel1, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), &thread_size, NULL);
printf("recommended workgroup size: %lu\n", thread_size);

/* --- enqueue the command to execute the kernel --------------------------- */

cl_event event_kernel1;
err = clEnqueueNDRangeKernel(queue, kernel1, 1, NULL, globalSize1, localSize1, 0, NULL, &event_kernel1);
if (err != CL_SUCCESS) {
  printf("error encountered while enqueuing the command to execute the kernel\n");
  exit(EXIT_FAILURE);
}
