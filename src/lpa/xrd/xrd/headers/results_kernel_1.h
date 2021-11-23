clWaitForEvents(1, &event_kernel1);

cl_ulong time_start_kernel1, time_stop_kernel1;

clGetEventProfilingInfo(event_kernel1, CL_PROFILING_COMMAND_START, sizeof(time_start_kernel1), &time_start_kernel1, NULL);
clGetEventProfilingInfo(event_kernel1, CL_PROFILING_COMMAND_END, sizeof(time_stop_kernel1), &time_stop_kernel1, NULL);

double time_kernel1 = time_stop_kernel1 - time_start_kernel1;

clEnqueueReadBuffer(queue, d_r1, CL_TRUE, 0, bytes_r1, h_r1, 0, NULL, NULL);
clEnqueueReadBuffer(queue, d_u1, CL_TRUE, 0, bytes_u1, h_u1, 0, NULL, NULL);
