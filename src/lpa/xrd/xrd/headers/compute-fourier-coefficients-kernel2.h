// globalsize and localsize for kernel 2
double time_kernel2 = 0.0f;
double cumulated_time_kernel2 = 0.0f;
size_t globalSize2[] = {Np};
size_t localSize2[] = {64};
int jind = 0;

FILE *FileFC;
FileFC = fopen(argv[6], "w");
printf("output file: %s\n", argv[6]);

fprintf(FileFC, "%8s # v: lpa-xrd version\n", "!VERSION");
fprintf(FileFC, "%8.2e # d: dislocation density [m^-2]\n", density);
fprintf(FileFC, "%2.0f %2.0f %2.0f # z: direction of 'l' (line vector) [uvw]\n", ld.x, ld.y, ld.z);
fprintf(FileFC, "%2.0f %2.0f %2.0f # b: Burgers vector direction [uvw]\n", bd.x*2/a_cell_param, bd.y*2/a_cell_param, bd.z*2/a_cell_param);
fprintf(FileFC, "%2.0f %2.0f %2.0f # g: diffraction vector direction (hkl)\n", H.x, H.y, H.z);
fprintf(FileFC, "%8f # C: contrast coefficient [1]\n", cfact_str);
fprintf(FileFC, "%8f # a: cell parameter [nm]\n", a_cell_param);
if (FLAG_SQUARE==1) {
  fprintf(FileFC, "%8.0f # s: side of the region of interest [nm]", Radius);
  if (D_REPLICATION>0) {
    fprintf(FileFC, " PBC%d", D_REPLICATION);
  }
  fprintf(FileFC, "\n");
} else {
  fprintf(FileFC, "%8.0f # s: radius of the region of interest [nm]\n", Radius);
}
fprintf(FileFC, "%8f # nu: Poisson's number [1]\n", nu);
fprintf(FileFC, "%8d # nd: number of dislocations in the input file\n", Nd0);
fprintf(FileFC, "%8d # np: number of random points\n", Np);
fprintf(FileFC, "# %4s", "L");

for (jind=1; jind<6; jind++){
  fprintf(FileFC, " %9s%d %9s%d %9s%d %9s%d", "cos", jind, "err_cos", jind, "sin", jind, "err_sin", jind);
}
fprintf(FileFC, " %10s %10s\n", "<eps^2>", "bad_points");

printf("loop over Fourier coefficients\n");

cumulated_time_kernel2 = 0.0f;

cl_double gs = length3(g);
//printf("gs: %lf\n", gs);

for (IndexFourier=1; IndexFourier<=NoFC; IndexFourier++) {
  //printf("IndexFourier: %d\n", IndexFourier);

  // define kernel 2 arguments
  err = clSetKernelArg(kernel2, 0, sizeof(cl_mem), &d_Vect16FC);
  err |= clSetKernelArg(kernel2, 1, sizeof(cl_mem), &d_rd0);
  err |= clSetKernelArg(kernel2, 2, sizeof(cl_mem), &d_r1);
  err |= clSetKernelArg(kernel2, 3, sizeof(cl_mem), &d_u1);
  err |= clSetKernelArg(kernel2, 4, sizeof(cl_double), &bed);
  err |= clSetKernelArg(kernel2, 5, sizeof(cl_double), &bsd);
  err |= clSetKernelArg(kernel2, 6, sizeof(cl_double), &Radius);
  err |= clSetKernelArg(kernel2, 7, sizeof(cl_double), &nu);
  err |= clSetKernelArg(kernel2, 8, sizeof(cl_int), &Np);
  err |= clSetKernelArg(kernel2, 9, sizeof(cl_int), &IndexFourier);
  err |= clSetKernelArg(kernel2, 10, sizeof(cl_double), &gs);
  err |= clSetKernelArg(kernel2, 11, sizeof(cl_double3), &gd);
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

  localSize2[0] = atoi(argv[2]); // 1 <= block <= 128 //
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

  double time_reduction = wtime();

  /* === compute the total number of useful points === */
  cl_int Np_good = Np;
  for(i=0; i<Np; i++) {
    Np_good -= (h_inout[i]==0)? 1:0;
  }
  //printf("Np      = %ld\n", Np);
  //printf("Np-good = %ld\n", Np_good);

  double eps2 = 0.0f;

  double c1AL=0.0f, c2AL=0.0f, c3AL=0.0f, c4AL=0.0f, c5AL=0.0f;
  double s1AL=0.0f, s2AL=0.0f, s3AL=0.0f, s4AL=0.0f, s5AL=0.0f;

  // compute by a reduction operation the errors for several Fourier Harmonic
  for (i=0; i< Np; i++) {
    // error coefficient
    eps2 += h_Vect16FC[i].sa;
    // for each harmonic compute the real and imaginary parts
    c1AL += h_Vect16FC[i].s0; s1AL += h_Vect16FC[i].s1;
    c2AL += h_Vect16FC[i].s2; s2AL += h_Vect16FC[i].s3;
    c3AL += h_Vect16FC[i].s4; s3AL += h_Vect16FC[i].s5;
    c4AL += h_Vect16FC[i].s6; s4AL += h_Vect16FC[i].s7;
    c5AL += h_Vect16FC[i].s8; s5AL += h_Vect16FC[i].s9;
  }

  time_reduction = wtime() - time_reduction;
  printf("time-reduction: %e s\n", time_reduction);

  double coeff = 1.0f/(double)(Np_good);
  printf("1/Np = %lf\n", coeff);

  double factor = (double)coeff;

  eps2 *= coeff;

  c1AL *= coeff; s1AL *= coeff;
  c2AL *= coeff; s2AL *= coeff;
  c3AL *= coeff; s3AL *= coeff;
  c4AL *= coeff; s4AL *= coeff;
  c5AL *= coeff; s5AL *= coeff;

  /* mean values of the harmonics */
  double mean_c1AL = c1AL;
  double mean_s1AL = s1AL;

  double mean_c2AL = c2AL;
  double mean_s2AL = s2AL;

  double mean_c3AL = c3AL;
  double mean_s3AL = s3AL;

  double mean_c4AL = c4AL;
  double mean_s4AL = s4AL;

  double mean_c5AL = c5AL;
  double mean_s5AL = s5AL;

  double err_C1 = 0.0f;
  double err_S1 = 0.0f;
  double err_C2 = 0.0f;
  double err_S2 = 0.0f;
  double err_C3 = 0.0f;
  double err_S3 = 0.0f;
  double err_C4 = 0.0f;
  double err_S4 = 0.0f;
  double err_C5 = 0.0f;
  double err_S5 = 0.0f;

  for (i=0; i< Np; i++) {
    if (h_inout[i]==1) {
      err_C1 += (h_Vect16FC[i].s0-mean_c1AL) * (h_Vect16FC[i].s0-mean_c1AL);
      err_S1 += (h_Vect16FC[i].s1-mean_s1AL) * (h_Vect16FC[i].s1-mean_s1AL);

      err_C2 += (h_Vect16FC[i].s2-mean_c2AL) * (h_Vect16FC[i].s2-mean_c2AL);
      err_S2 += (h_Vect16FC[i].s3-mean_s2AL) * (h_Vect16FC[i].s3-mean_s2AL);

      err_C3 += (h_Vect16FC[i].s4-mean_c3AL) * (h_Vect16FC[i].s4-mean_c3AL);
      err_S3 += (h_Vect16FC[i].s5-mean_s3AL) * (h_Vect16FC[i].s5-mean_s3AL);

      err_C4 += (h_Vect16FC[i].s6-mean_c4AL) * (h_Vect16FC[i].s6-mean_c4AL);
      err_S4 += (h_Vect16FC[i].s7-mean_s4AL) * (h_Vect16FC[i].s7-mean_s4AL);

      err_C5 += (h_Vect16FC[i].s8-mean_c5AL) * (h_Vect16FC[i].s8-mean_c5AL);
      err_S5 += (h_Vect16FC[i].s9-mean_s5AL) * (h_Vect16FC[i].s9-mean_s5AL);
    }
  }

  // compute the quadratic errors
  err_C1 = sqrt(err_C1)*coeff; err_S1 = sqrt(err_S1)*coeff;
  err_C2 = sqrt(err_C2)*coeff; err_S2 = sqrt(err_S2)*coeff;
  err_C3 = sqrt(err_C3)*coeff; err_S3 = sqrt(err_S3)*coeff;
  err_C4 = sqrt(err_C4)*coeff; err_S4 = sqrt(err_S4)*coeff;
  err_C5 = sqrt(err_C5)*coeff; err_S5 = sqrt(err_S5)*coeff;

  printf("c1AL=%9.6f err_C1=%9.6f\n", c1AL, err_C1);
  printf("s1AL=%9.6f err_S1=%9.6f\n", s1AL, err_S1);
  printf("c2AL=%9.6f err_C2=%9.6f\n", c2AL, err_C2);
  printf("s2AL=%9.6f err_S2=%9.6f\n", s2AL, err_S2);
  printf("c3AL=%9.6f err_C3=%9.6f\n", c3AL, err_C3);
  printf("s3AL=%9.6f err_S3=%9.6f\n", s3AL, err_S3);
  printf("c4AL=%9.6f err_C4=%9.6f\n", c4AL, err_C4);
  printf("s4AL=%9.6f err_S4=%9.6f\n", s4AL, err_S4);
  printf("c5AL=%9.6f err_C5=%9.6f\n", c5AL, err_C5);
  printf("s5AL=%9.6f err_S5=%9.6f\n", s5AL, err_S5);

  /*== Dump to File the Fourier coefficients ==*/
  fprintf(FileFC,
    "%6.1lf %10.7f %10.7f %10.7f %10.7f %10.7f %10.7f %10.7f %10.7f %10.7f %10.7f %10.7f %10.7f %10.7f %10.7f %10.7f %10.7f %10.7f %10.7f %10.7f %10.7f %10.7f %10d\n",
    IndexFourier*a3,
    c1AL, err_C1, s1AL, err_S1,
    c2AL, err_C2, s2AL, err_S2,
    c3AL, err_C3, s3AL, err_S3,
    c4AL, err_C4, s4AL, err_S4,
    c5AL, err_C5, s5AL, err_S5,
    eps2, Np-Np_good);
}
