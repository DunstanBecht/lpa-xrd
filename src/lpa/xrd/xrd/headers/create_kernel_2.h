// computation of the Fourier transform
cl_double16 *h_Vect16FC;
size_t bytes_Vect16FC = sizeof(cl_double16) * Np;

// variables on host (CPU)

// initialize to zero the Vect16FC
h_Vect16FC = (cl_double16 *)malloc(bytes_Vect16FC);
if (h_Vect16FC == NULL) {
  printf("memory allocation error for h_Vect16FC\n");
}
int mm;
for (mm=0; mm<Np; mm++) {
  h_Vect16FC[mm].s0 = 0.0f;
  h_Vect16FC[mm].s1 = 0.0f;
  h_Vect16FC[mm].s2 = 0.0f;
  h_Vect16FC[mm].s3 = 0.0f;
  h_Vect16FC[mm].s4 = 0.0f;
  h_Vect16FC[mm].s5 = 0.0f;
  h_Vect16FC[mm].s6 = 0.0f;
  h_Vect16FC[mm].s7 = 0.0f;
  h_Vect16FC[mm].s8 = 0.0f;
  h_Vect16FC[mm].s9 = 0.0f;
  h_Vect16FC[mm].sa = 0.0f;
  h_Vect16FC[mm].sb = 0.0f;
  h_Vect16FC[mm].sc = 0.0f;
  h_Vect16FC[mm].sd = 0.0f;
  h_Vect16FC[mm].se = 0.0f;
  h_Vect16FC[mm].sf = 0.0f;
}

// int array : 0 ou 1 : point inside or outside
size_t bytes_inout = sizeof(cl_int) * Np;
cl_int *h_inout;
h_inout = (cl_int *)malloc(bytes_inout);
if (h_inout == NULL) {
  printf("memory allocation error for h_inout\n");
}
for (mm=0; mm<Np; mm++) {
  h_inout[mm] = 0;
}

// variables on device (GPU)

cl_mem d_Vect16FC = clCreateBuffer(context, CL_MEM_READ_WRITE|CL_MEM_ALLOC_HOST_PTR, bytes_Vect16FC, NULL, NULL);
cl_mem d_inout = clCreateBuffer(context, CL_MEM_READ_WRITE|CL_MEM_ALLOC_HOST_PTR, bytes_inout, NULL, NULL);

// copy data from CPU to GPU for kernel 2

err = clEnqueueWriteBuffer(queue, d_Vect16FC, CL_TRUE, 0, bytes_Vect16FC, h_Vect16FC, 0, NULL, NULL);
err |= clEnqueueWriteBuffer(queue, d_inout, CL_TRUE, 0, bytes_inout, h_inout, 0, NULL, NULL);
if (err != CL_SUCCESS) {
  printf("clEnqueue error for kernel 2\n");
}

cl_kernel kernel2 = clCreateKernel(program, "comptf", &err);
if (err != CL_SUCCESS) {
  printf("error while creating kernel 2\n");
  exit(1);
}

clFinish(queue);
