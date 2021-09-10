  double time_kernel2=0.0f;
  double cumulated_time_kernel2=0.0f;
  size_t globalSize2[]={Np}; /* --  It is the number of Random Points -- */
  size_t localSize2[]={64}; /* optimal value */
    

  /* ================================== */
  /* Loop over all Fourier coefficients */
  printf("...\n");
  FILE *FileFC;
  FileFC=fopen(argv[6],"w");
  printf("Fourier Coefficient's FileName= %s\n",argv[6]);


  /*--- 23 mai 2019 : format de sortie --*/
  fprintf(FileFC,"%d\t# No of dislocations in the cell\n",Nd);
  fprintf(FileFC,"%d\t# Np of point-pairs\n",Np);
  if (FLAG_SQUARE==1)
     fprintf(FileFC,"%6.0f\t# Square\n",Radius);
  else
     fprintf(FileFC,"%6.0f\t# Radius\n",Radius);

  fprintf(FileFC,"%2.0f\t%2.0f\t%2.0f\t# diffraction vector, g:\n",H.x,H.y,H.z);
  fprintf(FileFC,"%2.0f\t%2.0f\t%2.0f\t# line vector, l:\n",ld.x,ld.y,ld.z);
  fprintf(FileFC,"%f\t%f\t%f\t# Burgers vector, b in [nm] \n",bd.x,bd.y,bd.z);
  fprintf(FileFC,"%f\t# C_factor/ nu = %f\n",cfact_str, nu);
  fprintf(FileFC,"%f\t# lattice parameter, [nm]\n",a_cell_param);
    fprintf(FileFC,"L\tcos_AL\terr_Cos\tsin_AL\terr_Sin\tCos_2AL\terr_Cos_2AL\tSin_2AL\terr_Sin_2AL\tCos_3AL\terr_Cos_3AL\tSin_3AL\terr_Sin_3AL\tCos_4AL\terr_Cos_4AL\tSin_4AL\terr_Sin_4AL\tCos_5AL\terr_Cos_5AL\tSin_5AL\terr_Sin_5AL\t<eps^2>\tbad_points\n");


  printf("\n... Start loop over Fourier Coefficients ...\n");
  cumulated_time_kernel2=0.0f;
  for ( IndexFourier=1 ; IndexFourier <= NoFC; IndexFourier++ )
  {
    /*== work index Fourier ==*/
    printf("--IndexFourier= %d\n",IndexFourier);
    
    /*== Enqueue the parameter IndexFourier which is not constant over the computation ==*/
    printf("  Enqueue parameter IndexFourier to GPU ?\n");

    /*== Define  the parameters of kernel 2 ==*/
    printf("  SetKernelArg to GPU\n");
    /*----------------------------------------
    *
    * Definition des arguments du kernel 2
    *
    -----------------------------------------*/
  
    cl_double gs=length3(g);
    printf("   gs= %lf\n",gs);

    err  = clSetKernelArg(kernel2,0,sizeof(cl_mem),&d_Vect16FC);
    if ( err != CL_SUCCESS )
    {
  	printf(" Erreur clSetKernelArg kernel2: arg 1\n");
  	exit(1);
    }	
    err  = clSetKernelArg(kernel2,1,sizeof(cl_mem),&d_Vect8FC);
    if ( err != CL_SUCCESS )
    {
  	printf(" Erreur clSetKernelArg kernel2: arg 1\n");
  	exit(1);
    }	
    err = clSetKernelArg(kernel2,2,sizeof(cl_mem),&d_Vect1FC);
    if ( err != CL_SUCCESS )
    {
  	printf(" Erreur clSetKernelArg kernel2: arg 2\n");
  	exit(1);
    }	
    err |= clSetKernelArg(kernel2,3,sizeof(cl_mem),&d_rd0);
    if ( err != CL_SUCCESS )
    {
  	printf(" Erreur clSetKernelArg kernel2: arg 3\n");
  	exit(1);
    }	
    err |= clSetKernelArg(kernel2,4,sizeof(cl_mem),&d_r1);
    err |= clSetKernelArg(kernel2,5,sizeof(cl_mem),&d_u1);
    err |= clSetKernelArg(kernel2,6,sizeof(cl_mem),&d_be);
    err |= clSetKernelArg(kernel2,7,sizeof(cl_mem),&d_bs);
    err |= clSetKernelArg(kernel2,8,sizeof(cl_double),&Radius);
    err |= clSetKernelArg(kernel2,9,sizeof(cl_double),&nu);
    err |= clSetKernelArg(kernel2,10,sizeof(cl_int),&Np); 
    err |= clSetKernelArg(kernel2,11,sizeof(cl_int),&IndexFourier); 
    err |= clSetKernelArg(kernel2,12,sizeof(cl_double),&gs);
    err |= clSetKernelArg(kernel2,13,sizeof(cl_double3),&gd);
    err |= clSetKernelArg(kernel2,14,sizeof(cl_double2),&a3vd);
    err |= clSetKernelArg(kernel2,15,sizeof(cl_double),&a3);
    err |= clSetKernelArg(kernel2,16,sizeof(cl_int),&Nd);                   
    err |= clSetKernelArg(kernel2,17,sizeof(cl_mem),&d_inout);                
    if ( err != CL_SUCCESS )
    {
  	printf(" Erreur clSetKernelArg 2\n");
  	exit(1);
    }	
    printf("Definition des arguments du Kernel 2 \n");

    /*== Enqueue Kernel ==*/
    printf("  Enqueue Kernel\n");
    /* ==== globalsize and localsize for kernel 2=== */
    cl_event event_kernel2;
    
    localSize2[0]=atoi(argv[2]); /* block value given by the user : 1 <= block <= 128 */
    printf("kernel2 : globalSize2= %lu\n",globalSize2[0]);
    printf("kernel2 : localSize2 = %lu\n",localSize2[0]);
  
    err = clEnqueueNDRangeKernel(queue,kernel2,1,NULL,globalSize2,localSize2,0,NULL,&event_kernel2);
    if ( err != CL_SUCCESS )
    {
  	printf(" Erreur definition NDRange pour kernel 2\n");
    }	

    printf("Enqueue Kernel2 done ....\n");

    /*== Define profiling event ==*/
    printf("  Define profiling event\n");
    /* ==== handle events to monitor kernel's execution number 2 === */
    clWaitForEvents(1, &event_kernel2);
    cl_ulong time_start_kernel2, time_stop_kernel2;
    clGetEventProfilingInfo(event_kernel2,CL_PROFILING_COMMAND_START,sizeof(time_start_kernel2),&time_start_kernel2,NULL); 
    clGetEventProfilingInfo(event_kernel2,CL_PROFILING_COMMAND_END,sizeof(time_stop_kernel2),&time_stop_kernel2,NULL);     

    /*== Get profiling data from kernel 2==*/
    printf("  Get profiling data from kernel 2\n");
    time_kernel2=time_stop_kernel2-time_start_kernel2;
    cumulated_time_kernel2 +=time_kernel2;
    printf("Execution timekernel2 : %0.3f ms\n",time_kernel2*1.0e-6);

    /* ==== read-back computational results par le kernel 2 === */
    printf("  Read Back from gpu arrays VECT16FC, VECT8FC and VECT1FC\n");
    clEnqueueReadBuffer(queue,d_Vect16FC, CL_TRUE, 0, bytes_Vect16FC, h_Vect16FC, 0, NULL, NULL);
    clEnqueueReadBuffer(queue,d_Vect8FC, CL_TRUE, 0, bytes_Vect8FC, h_Vect8FC, 0, NULL, NULL);
    clEnqueueReadBuffer(queue,d_Vect1FC, CL_TRUE, 0, bytes_Vect1FC, h_Vect1FC, 0, NULL, NULL);
    clEnqueueReadBuffer(queue,d_inout, CL_TRUE, 0, bytes_inout, h_inout, 0, NULL, NULL);
  
    printf(".. On a fini avec le kernel 2 \n");

    /*== Dump the two arrays to file ==*/
    if ( DUMP == 1)
    {

      printf("  Dump arrayinouts VECT8FC and VECT1FC to file\n");
      char str[80];
      char number[7];
      strcpy(str,"VECT8FC_");
      sprintf(number,"%03d",IndexFourier);
      strcat(str,number);
      strcat(str,".txt");
      FILE *filev;
      filev=fopen(str,"w");
      for (i=0; i< Np; i++)
      {
        fprintf(filev,"%lf ",h_Vect8FC[i].s0);
        fprintf(filev,"%lf ",h_Vect8FC[i].s1);
        fprintf(filev,"%lf ",h_Vect8FC[i].s2);
        fprintf(filev,"%lf ",h_Vect8FC[i].s3);
        fprintf(filev,"%lf ",h_Vect8FC[i].s4);
        fprintf(filev,"%lf ",h_Vect8FC[i].s5);
        fprintf(filev,"%lf ",h_Vect8FC[i].s6);
        fprintf(filev,"%lf\n",h_Vect8FC[i].s7);
      }  
      fclose(filev);		

      strcpy(str,"VECT1FC_");
      sprintf(number,"%03d",IndexFourier);
      strcat(str,number);
      strcat(str,".txt");
      filev=fopen(str,"w");
      for (i=0; i< Np; i++)
        fprintf(filev,"%lf\n",h_Vect1FC[i]);
      fclose(filev);
    }
    printf("---cumulated-time-kernel2 [ms]=%lf\n",cumulated_time_kernel2*1.0e-6);
