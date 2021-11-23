cl_kernel kernel2; // kernel object
cl_double16 *h_Vect16FC; // vector for storing calculated values in the kernel
cl_int *h_inout; // random point inside (1) or outside (0) the region of interest

/* --- create data structures on the host ---------------------------------- */

size_t bytes_Vect16FC = sizeof(cl_double16) * Np;
h_Vect16FC = (cl_double16 *)malloc(bytes_Vect16FC);
if (h_Vect16FC == NULL) {
  printf("error encountered during memory allocation for h_Vect16FC\n");
  exit(EXIT_FAILURE);
}
for (i=0; i<Np; i++) {
  for (j=0; j<16; j++) {
    h_Vect16FC[i].s[j] = 0.0f;
  }
}

size_t bytes_inout = sizeof(cl_int) * Np;
h_inout = (cl_int *)malloc(bytes_inout);
if (h_inout == NULL) {
  printf("error encountered during memory allocation for h_inout\n");
  exit(EXIT_FAILURE);
}
for (i=0; i<Np; i++) {
  h_inout[i] = 0;
}

/* --- create buffer objects ----------------------------------------------- */

cl_mem d_Vect16FC = clCreateBuffer(context, CL_MEM_READ_WRITE|CL_MEM_ALLOC_HOST_PTR, bytes_Vect16FC, NULL, NULL);
cl_mem d_inout = clCreateBuffer(context, CL_MEM_READ_WRITE|CL_MEM_ALLOC_HOST_PTR, bytes_inout, NULL, NULL);

/* --- write to buffer objects from host memory ---------------------------- */

err = clEnqueueWriteBuffer(queue, d_Vect16FC, CL_TRUE, 0, bytes_Vect16FC, h_Vect16FC, 0, NULL, NULL);
err |= clEnqueueWriteBuffer(queue, d_inout, CL_TRUE, 0, bytes_inout, h_inout, 0, NULL, NULL);
if (err != CL_SUCCESS) {
  printf("error encountered while writing to buffer objects from host memory\n");
  exit(EXIT_FAILURE);
}

/* --- create a kernel object ---------------------------------------------- */

kernel2 = clCreateKernel(program, "comptf", &err);
if (err != CL_SUCCESS) {
  printf("error encountered during kernel creation\n");
  exit(EXIT_FAILURE);
}

clFinish(queue);
