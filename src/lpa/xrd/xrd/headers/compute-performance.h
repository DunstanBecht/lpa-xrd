  /*== doubleing point done by kernel 1 ==*/
  double op_kernel1=Np*(2.0+Nd*20.0f);
  printf("double-kernel1= %e\n",op_kernel1);
  printf("GflopsKernel1 = %lf\n",op_kernel1/(time_kernel1*1.0e-9)*1.0e-9);
  
  printf("%ld %e\n",Np,time_kernel1*1.0e-9);

  /*== doubleing point done by kernel 2 ==*/
  double op_kernel2=Np*(1.0+Nd*41.0f);
  printf("double-kernel2= %e\n",op_kernel2);
  printf("GflopsKernel2 = %lf\n",op_kernel2/(time_kernel2*1.0e-9)*1.0e-9);

  FILE *file;
  file=fopen("duree-run-kernel1.txt","a+");
  fprintf(file,"%d %ld %ld %e %lf\n",gpu,localSize1[0],Np,time_kernel1*1.0e-9,op_kernel1/(time_kernel1*1.0e-9)*1.0e-9);
  fclose(file);
  file=fopen("duree-run-kernel2.txt","a+");
  fprintf(file,"%d %ld %ld %e %lf\n",gpu,localSize2[0],Np,time_kernel2*1.0e-9,op_kernel2/(time_kernel2*1.0e-9)*1.0e-9);
  fclose(file);
  
  printf("---cumulated-time-kernel2 [ms]=%lf for computation of all Fourier Coefficients\n",cumulated_time_kernel2*1.0e-6);
  printf("---program ended successfully-------------\n");
  printf("...version 28th june 2021 : compute 3rd, 4th, 5th Harmonic of Fourier Coefficient\n");
  return 0;
}

