  /* ==== handle events to monitor kernel's execution === */
  clWaitForEvents(1, &event_kernel1);
 
  cl_ulong time_start_kernel1, time_stop_kernel1;

 clGetEventProfilingInfo(event_kernel1,CL_PROFILING_COMMAND_START,sizeof(time_start_kernel1),&time_start_kernel1,NULL); 
  clGetEventProfilingInfo(event_kernel1,CL_PROFILING_COMMAND_END,sizeof(time_stop_kernel1),&time_stop_kernel1,NULL); 

  double time_kernel1=time_stop_kernel1-time_start_kernel1;
  printf("Execution timekernel1 : %0.3f ms\n",time_kernel1*1.0e-6);

  /* ==== read-back computational results par le kernel 1 === */
  printf("On lit les champs de vecteurs calcules par kernel 1\n");
  clEnqueueReadBuffer(queue,d_r1, CL_TRUE, 0, bytes_r1, h_r1, 0, NULL, NULL);
  clEnqueueReadBuffer(queue,d_u1, CL_TRUE, 0, bytes_u1, h_u1, 0, NULL, NULL);
  
  printf(".. On a fini avec le kernel 1 /n");

