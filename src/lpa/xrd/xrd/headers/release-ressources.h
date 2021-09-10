 
  /* ==== On libere les ressources allouees === */
  // Clean up
  ret = clFlush(queue);
  ret = clFinish(queue);
  ret = clReleaseKernel(kernel1);
  ret = clReleaseProgram(program);
  ret = clReleaseCommandQueue(queue);
  ret = clReleaseContext(context);
  printf("Free cl ressources\n");
  /* free memory references from device*/
  ret = clReleaseMemObject(d_rd0);
  ret = clReleaseMemObject(d_ranangle);
  ret = clReleaseMemObject(d_ranradius);
  ret = clReleaseMemObject(d_ranx);
  ret = clReleaseMemObject(d_rany);
  ret = clReleaseMemObject(d_r1);
  ret = clReleaseMemObject(d_u1);
  ret = clReleaseMemObject(d_be);
  ret = clReleaseMemObject(d_bs);
  ret = clReleaseMemObject(d_inout);
  ret = clReleaseMemObject(d_Vect1FC);
  ret = clReleaseMemObject(d_Vect8FC);
  ret = clReleaseMemObject(d_Vect16FC);

  printf("Free memory references from device\n");
  /* free memory references from host*/
  free(sd0);
  free(rd0);
  free(be);
  free(bs);
  free(RandAngle);
  free(RandRadius);
  free(RandX);
  free(RandY);
  free(h_r1);
  free(h_u1);
  free(h_inout);
  free(h_Vect1FC);
  free(h_Vect8FC);
  free(h_Vect16FC);
  printf("Free memory references from host\n");

  printf("\n");

