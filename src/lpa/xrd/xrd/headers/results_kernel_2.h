double time_kernel2 = 0.0f;
double cumulated_time_kernel2 = 0.0f;
size_t globalSize2[] = {Np};
size_t localSize2[] = {64};

double res_cos[Nf][HARMONICS]; // real part of the Fourier transform
double res_sin[Nf][HARMONICS]; // imaginary part of the Fourier transform
double res_cos_std[Nf][HARMONICS]; // error off cos coefficients
double res_sin_std[Nf][HARMONICS]; // error off sin coefficients
double res_eps[Nf]; // mean square strain
int res_nrp[Nf]; // number of random points inside the region of interest

double residual; // difference between a value and the mean

printf("loop over Fourier coefficients\n");

g_vec_len = length3(g_vec);

cumulated_time_kernel2 = 0.0f;

for (i=1; i<=Nf; i++) { // L = a3 * i

  for (j=0; j<HARMONICS; j++) {
    res_cos[i-1][j] = 0;
    res_sin[i-1][j] = 0;
    res_cos_std[i-1][j] = 0;
    res_sin_std[i-1][j] = 0;
  }
  res_eps[i-1] = 0;
  res_nrp[i-1] = 0;

  err = clSetKernelArg(kernel2, 0, sizeof(cl_mem), &d_Vect16FC);
  err |= clSetKernelArg(kernel2, 1, sizeof(cl_mem), &d_dislocations);
  err |= clSetKernelArg(kernel2, 2, sizeof(cl_mem), &d_r1);
  err |= clSetKernelArg(kernel2, 3, sizeof(cl_mem), &d_u1);
  err |= clSetKernelArg(kernel2, 4, sizeof(cl_double), &be_len);
  err |= clSetKernelArg(kernel2, 5, sizeof(cl_double), &bs_len);
  err |= clSetKernelArg(kernel2, 6, sizeof(cl_double), &size);
  err |= clSetKernelArg(kernel2, 7, sizeof(cl_double), &nu);
  err |= clSetKernelArg(kernel2, 8, sizeof(cl_int), &Np);
  err |= clSetKernelArg(kernel2, 9, sizeof(cl_int), &i);
  err |= clSetKernelArg(kernel2, 10, sizeof(cl_double), &g_vec_len);
  err |= clSetKernelArg(kernel2, 11, sizeof(cl_double3), &gd_vec);
  err |= clSetKernelArg(kernel2, 12, sizeof(cl_double2), &a3vd);
  err |= clSetKernelArg(kernel2, 13, sizeof(cl_double), &a3);
  err |= clSetKernelArg(kernel2, 14, sizeof(cl_int), &Nd);
  err |= clSetKernelArg(kernel2, 15, sizeof(cl_int), &FLAG_SQUARE);
  err |= clSetKernelArg(kernel2, 16, sizeof(cl_mem), &d_inout);
  err |= clSetKernelArg(kernel2, 17, shared_size_kernel1, NULL);

  if (err != CL_SUCCESS) {
    printf("clSetKernelArg error for kernel 2\n");
    exit(1);
  }

  // equeue kernel 2
  cl_event event_kernel2;

  localSize2[0] = atoi(argv[2]); // 1 <= block <= 128
  //printf("kernel 2 globalSize: %lu\n", globalSize2[0]);
  //printf("kernel 2 locaSize: %lu\n", localSize2[0]);

  err = clEnqueueNDRangeKernel(queue, kernel2, 1, NULL, globalSize2, localSize2, 0, NULL, &event_kernel2);
  if (err != CL_SUCCESS) {
    printf("NDRange definition error for kernel 2\n");
  }

  // handle events to monitor kernel's execution
  clWaitForEvents(1, &event_kernel2);
  cl_ulong time_start_kernel2, time_stop_kernel2;
  clGetEventProfilingInfo(event_kernel2, CL_PROFILING_COMMAND_START, sizeof(time_start_kernel2), &time_start_kernel2, NULL);
  clGetEventProfilingInfo(event_kernel2, CL_PROFILING_COMMAND_END, sizeof(time_stop_kernel2), &time_stop_kernel2, NULL);

  time_kernel2 = time_stop_kernel2 - time_start_kernel2;
  cumulated_time_kernel2 += time_kernel2;
  //printf("kernel 2 execution time: %0.3f ms\n", time_kernel2*1.0e-6);

  clEnqueueReadBuffer(queue, d_Vect16FC, CL_TRUE, 0, bytes_Vect16FC, h_Vect16FC, 0, NULL, NULL);
  clEnqueueReadBuffer(queue, d_inout, CL_TRUE, 0, bytes_inout, h_inout, 0, NULL, NULL);

  //printf("kernel 2 execution cumulated time: %lf\n", cumulated_time_kernel2*1.0e-6);

  // compute the total number of useful points
  res_nrp[i-1] = Np;
  for(k=0; k<Np; k++) {
    if (h_inout[k]==0) {
      res_nrp[i-1] -= 1;
    }
  }

  // sum
  for (k=0; k<Np; k++) {
    res_eps[i-1] += h_Vect16FC[k].sa;
    for (j=0; j<5; j++) {
      res_cos[i-1][j] += h_Vect16FC[k].s[2*j];
      res_sin[i-1][j] += h_Vect16FC[k].s[2*j+1];
    }
  }

  // average
  for (j=0; j<HARMONICS; j++) {
    res_cos[i-1][j] /= res_nrp[i-1];
    res_sin[i-1][j] /= res_nrp[i-1];
  }
  res_eps[i-1] /= res_nrp[i-1];

  // error
  for (j=0; j<HARMONICS; j++) {
    for (k=0; k<Np; k++) {
      if (h_inout[k]==1) {
        residual = h_Vect16FC[k].s[2*j] - res_cos[i-1][j];
        res_cos_std[i-1][j] += residual * residual;
        residual = h_Vect16FC[k].s[2*j+1] - res_sin[i-1][j];
        res_sin_std[i-1][j] += residual * residual;
      }
    }
    res_cos_std[i-1][j] = sqrt(res_cos_std[i-1][j]/(res_nrp[i-1]-1));
    res_sin_std[i-1][j] = sqrt(res_sin_std[i-1][j]/(res_nrp[i-1]-1));
  }
}
