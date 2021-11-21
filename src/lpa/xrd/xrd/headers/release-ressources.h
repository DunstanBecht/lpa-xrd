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
