// dump results to file for debugging
if (DUMP == 1){

  FILE *fr;
  fr = fopen("r.res", "w");
  for (i=0; i<Np; i++) {
    fprintf(fr, "%lf %lf\n", h_r1[i].x, h_r1[i].y);
  }
  fclose(fr);
  printf("dump 2d position field\n");

  FILE *fu;
  fu = fopen("u.res", "w");
  for (i=0; i<Np; i++) {
    fprintf(fu, "%lf %lf %lf\n", h_u1[i].x, h_u1[i].y, h_u1[i].z);
  }
  fclose(fu);
  printf("dump 3d displacement field\n");

}

ret = clFlush(queue);
ret |= clFinish(queue);
ret |= clReleaseKernel(kernel1);
ret |= clReleaseKernel(kernel2);
ret |= clReleaseProgram(program);
ret |= clReleaseCommandQueue(queue);
ret |= clReleaseContext(context);

if (ret != CL_SUCCESS) {
  printf("clRelease error\n");
  exit(1);
}

ret = clReleaseMemObject(d_rd0);
ret |= clReleaseMemObject(d_ranangle);
ret |= clReleaseMemObject(d_ranradius);
ret |= clReleaseMemObject(d_ranx);
ret |= clReleaseMemObject(d_rany);
ret |= clReleaseMemObject(d_inout);
ret |= clReleaseMemObject(d_r1);
ret |= clReleaseMemObject(d_u1);
ret |= clReleaseMemObject(d_Vect16FC);

if (ret != CL_SUCCESS) {
  printf("clReleaseMemObject error\n");
  exit(1);
}

printf("free memory references from device\n");

free(sd0);
free(rd0);
free(rd0_all);
free(random1);
free(random2);
free(h_r1);
free(h_u1);
free(h_inout);
free(h_Vect16FC);

printf("free memory references from host\n");
