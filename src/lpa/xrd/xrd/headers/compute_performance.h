//  double precision floating point operations done by kernel 1
double op_kernel1 = Np * (2.0+Nd*20.0f);
printf("operations done by kernel 1: %e\n", op_kernel1);
printf("floating point operations per second: %lf GFLOPS\n", op_kernel1/(time_kernel1*1.0e-9)*1.0e-9);
printf("duration: %e s\n", time_kernel1*1.0e-9);

// double precision floating point operations done by kernel 2
double op_kernel2 = Np * (24.0+Nd*27.0f);
printf("operations done by kernel 2: %e\n",op_kernel2);
printf("floating point operations per second: %lf GFLOPS\n", op_kernel2/(time_kernel2*1.0e-9)*1.0e-9);
printf("duration: %lf s (cumulated for all Fourier coefficients)\n", cumulated_time_kernel2*1.0e-9);

printf("program ended successfully");

return 0;
