// computation of the resulting displacement field
cl_kernel kernel1 = clCreateKernel(program, "udislo", &err);
if (err != CL_SUCCESS) {
  printf("compilation of kernel 1 error on the GPU\n");
  exit(11);
}

// create host output structure for the results
cl_double2 *h_r1; // random point positions

h_r1 = (cl_double2 *)malloc(sizeof(cl_double2)*Np);
if (h_r1 == NULL) {
  printf("memory allocation error for h_r1\n");
}
printf("allocation host h_r1: %lu Bytes\n", sizeof(cl_double2)*Np);

cl_double3 *h_u1; // displacement field at the random points

h_u1 = (cl_double3 *)malloc(sizeof(cl_double3)*Np);
if (h_u1 == NULL) {
  printf("memory allocation error for h_u1\n");
}
printf("allocation host h_u1: %lu Bytes\n", sizeof(cl_double3)*Np);

// create input and output device buffers for kernel 1

// the dislocations are stored as a 3-vector where z component is the Burgers vector sense
size_t bytes_rd0 = sizeof(cl_double3) * Nd;
cl_mem d_rd0 = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_ALLOC_HOST_PTR, bytes_rd0, NULL, NULL);

size_t bytes_ran = sizeof(cl_double) * Np;
cl_mem d_ranradius = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_ALLOC_HOST_PTR, bytes_ran, NULL, NULL);
cl_mem d_ranangle = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_ALLOC_HOST_PTR, bytes_ran, NULL, NULL);

cl_mem d_ranx = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_ALLOC_HOST_PTR, bytes_ran, NULL, NULL);
cl_mem d_rany = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_ALLOC_HOST_PTR, bytes_ran, NULL, NULL);

size_t bytes_r1 = sizeof(cl_double2) * Np;
cl_mem d_r1 = clCreateBuffer(context, CL_MEM_READ_WRITE|CL_MEM_ALLOC_HOST_PTR, bytes_r1, NULL, NULL);

size_t bytes_u1 = sizeof(cl_double3) * Np;
cl_mem d_u1 = clCreateBuffer(context, CL_MEM_READ_WRITE|CL_MEM_ALLOC_HOST_PTR, bytes_u1, NULL, NULL);

printf("create input and output device buffers for Kernel 1\n");

// copy data from host (cpu) to device (gpu) for kernel 1

err = clEnqueueWriteBuffer(queue, d_rd0, CL_TRUE, 0, bytes_rd0, rd0_all, 0, NULL, NULL);

if (FLAG_SQUARE == 1){
  err |= clEnqueueWriteBuffer(queue,d_ranx, CL_TRUE, 0, bytes_ran, RandX, 0, NULL, NULL);
  err |= clEnqueueWriteBuffer(queue,d_rany, CL_TRUE, 0, bytes_ran,RandY, 0, NULL, NULL);
} else {
  err |= clEnqueueWriteBuffer(queue,d_ranradius, CL_TRUE, 0, bytes_ran,RandRadius, 0, NULL, NULL);
  err |= clEnqueueWriteBuffer(queue, d_ranangle, CL_TRUE, 0, bytes_ran, RandAngle, 0, NULL, NULL);
}

err |= clEnqueueWriteBuffer(queue, d_r1, CL_TRUE, 0, bytes_r1, h_r1, 0, NULL, NULL);
err |= clEnqueueWriteBuffer(queue, d_u1, CL_TRUE, 0, bytes_u1, h_u1, 0, NULL, NULL);

if (err != CL_SUCCESS) {
  printf("clEnqueueWriteBuffer error for kernel 1\n");
}

// globalsize and localsize for kernel 1
size_t globalSize1[] = {Np};
size_t localSize1[] = {64}; // optimal value by default
localSize1[0] = atoi(argv[2]); // choice of the local size by the parameter */
printf("kernel 1 globalSize: %lu\n", globalSize1[0]);
printf("kernel 1 localSize: %lu\n", localSize1[0]);

// definition of the shared size to use the shared variables
size_t shared_size_kernel1;
shared_size_kernel1 = localSize1[0] * sizeof(cl_double3);
printf("shared_size_kernel1: %d Bytes\n", shared_size_kernel1);

// arguments definition (transfert data to GPU)
if (FLAG_SQUARE == 1) {
  err = clSetKernelArg(kernel1, 0, sizeof(cl_mem), &d_ranx);
  err |= clSetKernelArg(kernel1, 1, sizeof(cl_mem), &d_rany);
} else {
  err = clSetKernelArg(kernel1, 0, sizeof(cl_mem), &d_ranradius);
  err |= clSetKernelArg(kernel1, 1, sizeof(cl_mem), &d_ranangle);
}
err |= clSetKernelArg(kernel1, 2, sizeof(cl_mem), &d_rd0);
err |= clSetKernelArg(kernel1, 3, sizeof(cl_mem), &d_r1);
err |= clSetKernelArg(kernel1, 4, sizeof(cl_mem), &d_u1);
err |= clSetKernelArg(kernel1, 5, sizeof(cl_double), &bed);
err |= clSetKernelArg(kernel1, 6, sizeof(cl_double), &bsd);
err |= clSetKernelArg(kernel1, 7, sizeof(cl_double), &Radius);
err |= clSetKernelArg(kernel1, 8, sizeof(cl_double), &nu);
err |= clSetKernelArg(kernel1, 9, sizeof(cl_int), &Np);
err |= clSetKernelArg(kernel1, 10, sizeof(cl_int), &Nd);
err |= clSetKernelArg(kernel1, 11, sizeof(cl_int), &FLAG_SQUARE);
err |= clSetKernelArg(kernel1, 12, shared_size_kernel1, NULL); // size of the used shared memory

if (err != CL_SUCCESS) {
  printf("clSetKernelArg error for kernel 1\n");
  exit(1);
}
printf("kernel 1 arguments definition done\n");

// get the recommended thread size for kernel 1
size_t thread_size;
clGetKernelWorkGroupInfo(kernel1, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), &thread_size, NULL);
printf("recommended workgroup size: %lu\n", thread_size);

// globalsize and localsize for kernel 1
cl_event event_kernel1;

err = clEnqueueNDRangeKernel(queue, kernel1, 1, NULL, globalSize1, localSize1, 0, NULL, &event_kernel1);
if (err != CL_SUCCESS) {
  printf("clEnqueue error for Kernel 1\n");
  exit(1);
}
