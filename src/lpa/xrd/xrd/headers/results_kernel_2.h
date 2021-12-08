double output_cos[Nf][HARMONICS]; // estimator of the mean cos coefficients of the Fourier transform
double output_sin[Nf][HARMONICS]; // estimator of the mean sin coefficients of the Fourier transform
double output_err_cos[Nf][HARMONICS]; // estimator of the standard deviation of the cos mean (standard error)
double output_err_sin[Nf][HARMONICS]; // estimator of the standard deviation of the sin mean (standard error)
double output_eps2[Nf]; // estimator of the mean square strain
int output_nrpi[Nf]; // number of random points inside the region of interest
double residual; // difference between a value and the mean

printf("loop over Fourier coefficients\n");

g_vec_len = length3(g_vec);

for (i=0; i<Nf; i++) { // L = a3 * i

  for (j=0; j<HARMONICS; j++) {
    output_cos[i][j] = 0;
    output_sin[i][j] = 0;
    output_err_cos[i][j] = 0;
    output_err_sin[i][j] = 0;
  }
  output_eps2[i] = 0;
  output_nrpi[i] = 0;

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
    printf("error encountered while setting the value of the kernel arguments\n");
    exit(EXIT_FAILURE);
  }

  // equeue kernel 2
  cl_event event_kernel2;

  err = clEnqueueNDRangeKernel(queue, kernel2, 1, NULL, globalSize2, localSize2, 0, NULL, &event_kernel2);
  if (err != CL_SUCCESS) {
    printf("error encountered while enqueuing the command to execute the kernel\n");
    exit(EXIT_FAILURE);
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
  output_nrpi[i] = Np;
  for(k=0; k<Np; k++) {
    if (h_inout[k]==0) {
      output_nrpi[i] -= 1;
    }
  }

  // sum
  for (k=0; k<Np; k++) {
    output_eps2[i] += h_Vect16FC[k].sa;
    for (j=0; j<5; j++) {
      output_cos[i][j] += h_Vect16FC[k].s[2*j];
      output_sin[i][j] += h_Vect16FC[k].s[2*j+1];
    }
  }

  // average
  for (j=0; j<HARMONICS; j++) {
    output_cos[i][j] /= output_nrpi[i];
    output_sin[i][j] /= output_nrpi[i];
  }
  output_eps2[i] /= output_nrpi[i];

  // error
  for (j=0; j<HARMONICS; j++) {
    for (k=0; k<Np; k++) {
      if (h_inout[k]==1) {
        residual = h_Vect16FC[k].s[2*j] - output_cos[i][j];
        output_err_cos[i][j] += residual * residual;
        residual = h_Vect16FC[k].s[2*j+1] - output_sin[i][j];
        output_err_sin[i][j] += residual * residual;
      }
    }
    output_err_cos[i][j] = sqrt(output_err_cos[i][j]/(output_nrpi[i]-1)/output_nrpi[i]);
    output_err_sin[i][j] = sqrt(output_err_sin[i][j]/(output_nrpi[i]-1)/output_nrpi[i]);
  }
}
