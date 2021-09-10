
  /*
  ------------ Creation du premier kernel : udislo
  */ 
  cl_kernel kernel1 = clCreateKernel(program,"udislo", &err);
  if ( err != CL_SUCCESS )
  {
    printf("...ERREUR compilation du kernel 1 sur le gpu\n");
    exit(11);
  }    
  printf("Creation du Kernel 1 :-- Calcul du champ de deplacement aleatoire\n");
  
  /* ==== Creation des hosts output structure qui contiendront le resultat du calcul sur gpu === */
  cl_double2 *h_r1; //-- positions 2D generees aleatoirement

  h_r1 = (cl_double2 *)malloc(sizeof(cl_double2)*Np);
  if ( h_r1 == NULL )
  {
    printf("Erreur allocation mem h_r1\n");
  }
  printf("Allocation host : h_r1 de %lu  Bytes\n",sizeof(cl_double2)*Np);
  printf("Allocation host : h_r1 de %lu kBytes\n",sizeof(cl_double2)*Np/1024);

  cl_double3 *h_u1; //-- champ de deplacement 3d induit par la distribution 2d precedente
  
  h_u1 = (cl_double3 *)malloc(sizeof(cl_double3)*Np);
  if ( h_u1 == NULL )
  {
    printf("Erreur allocation mem h_u1\n");
  }
  printf("Allocation host : h_u1 de %lu Bytes\n",sizeof(cl_double3)*Np);

  /* ==== Creation des inputs et output device buffers kernel 1=== */
  // Essentiellement des tableaux de vecteurs 2d et 3d

  size_t  bytes_rd0= sizeof(cl_double2)*Nd;
  cl_mem   d_rd0=clCreateBuffer(context,CL_MEM_READ_ONLY,bytes_rd0,NULL,NULL);
 
  size_t bytes_ran= sizeof(cl_double)*Np;
  cl_mem  d_ranradius=clCreateBuffer(context,CL_MEM_READ_ONLY|CL_MEM_ALLOC_HOST_PTR,bytes_ran,NULL,NULL);
  cl_mem  d_ranangle=clCreateBuffer(context,CL_MEM_READ_ONLY|CL_MEM_ALLOC_HOST_PTR,bytes_ran,NULL,NULL);

  cl_mem  d_ranx=clCreateBuffer(context,CL_MEM_READ_ONLY|CL_MEM_ALLOC_HOST_PTR,bytes_ran,NULL,NULL);
  cl_mem  d_rany=clCreateBuffer(context,CL_MEM_READ_ONLY|CL_MEM_ALLOC_HOST_PTR,bytes_ran,NULL,NULL);

  size_t  bytes_r1= sizeof(cl_double2)*Np;
  cl_mem   d_r1=clCreateBuffer(context,CL_MEM_READ_WRITE|CL_MEM_ALLOC_HOST_PTR,bytes_r1,NULL,NULL);

  size_t  bytes_u1= sizeof(cl_double3)*Np;
  cl_mem   d_u1=clCreateBuffer(context,CL_MEM_READ_WRITE|CL_MEM_ALLOC_HOST_PTR,bytes_u1,NULL,NULL);

  size_t  bytes_be= sizeof(cl_double)*Nd;
  cl_mem   d_be=clCreateBuffer(context,CL_MEM_READ_ONLY,bytes_be,NULL,NULL);

  size_t  bytes_bs= sizeof(cl_double)*Nd;
  cl_mem   d_bs=clCreateBuffer(context,CL_MEM_READ_ONLY,bytes_bs,NULL,NULL);
  
  printf("Create input and output device buffers for Kernel 1\n");

  /* ==== Copie des donnees du host(cpu) vers le device(gpu)  kernel 1 === */
  //cl_int err; /* variable err */
 
  err  = clEnqueueWriteBuffer(queue,d_rd0 ,CL_TRUE,0,bytes_rd0 ,rd0_all,0,NULL,NULL);
  /** Tester en fonction du type de maillage: cylindrique ou carre **/
  if ( FLAG_SQUARE == 1 ) 
  {
     printf("...ENQUEUE FLAG_SQUARE=1\n");
     err |= clEnqueueWriteBuffer(queue,d_ranx,CL_TRUE,0,bytes_ran,RandX,0,NULL,NULL);
     err |= clEnqueueWriteBuffer(queue,d_rany,CL_TRUE,0,bytes_ran,RandY,0,NULL,NULL);
  }
  else
  {
    err |= clEnqueueWriteBuffer(queue,d_ranradius,CL_TRUE,0,bytes_ran,RandRadius,0,NULL,NULL);
    err |= clEnqueueWriteBuffer(queue,d_ranangle,CL_TRUE,0,bytes_ran,RandAngle,0,NULL,NULL);
  }
  err |= clEnqueueWriteBuffer(queue,d_r1 ,CL_TRUE,0,bytes_r1 ,h_r1 ,0,NULL,NULL);
  err |= clEnqueueWriteBuffer(queue,d_u1 ,CL_TRUE,0,bytes_u1 ,h_u1 ,0,NULL,NULL);
  err |= clEnqueueWriteBuffer(queue,d_be ,CL_TRUE,0,bytes_be ,be ,0,NULL,NULL);
  err |= clEnqueueWriteBuffer(queue,d_bs ,CL_TRUE,0,bytes_bs ,bs ,0,NULL,NULL);
  if ( err != CL_SUCCESS )
  {
  	printf(" Erreur Enqueue Write Buffer kernel 1\n");
  }	
  printf("Definition des buffers mem sur le gpu et copie des donnees Kernel 1\n");

  /* ==== Definition des arguments du compute Kernel : kernel 1=== */
  if ( FLAG_SQUARE == 1 ) 
  {
    printf("...FLAG_SQUARE=1 transfert data to gpu");
    err  = clSetKernelArg(kernel1,0,sizeof(cl_mem),&d_ranx);
    err |= clSetKernelArg(kernel1,1,sizeof(cl_mem),&d_rany);
  }
  else /* FLAG_SQUARE=0 i.e. cylindrical */
  {
    err  = clSetKernelArg(kernel1,0,sizeof(cl_mem),&d_ranradius);
    err |= clSetKernelArg(kernel1,1,sizeof(cl_mem),&d_ranangle);
  }
  err |= clSetKernelArg(kernel1,2,sizeof(cl_mem),&d_rd0);
  err |= clSetKernelArg(kernel1,3,sizeof(cl_mem),&d_r1);
  err |= clSetKernelArg(kernel1,4,sizeof(cl_mem),&d_u1);
  err |= clSetKernelArg(kernel1,5,sizeof(cl_mem),&d_be);
  err |= clSetKernelArg(kernel1,6,sizeof(cl_mem),&d_bs);
  err |= clSetKernelArg(kernel1,7,sizeof(cl_double),&Radius);
  err |= clSetKernelArg(kernel1,8,sizeof(cl_double),&nu);
  err |= clSetKernelArg(kernel1,9,sizeof(cl_int),&Np); 
  err |= clSetKernelArg(kernel1,10,sizeof(cl_int),&Nd); 
  err |= clSetKernelArg(kernel1,11,sizeof(cl_int),&FLAG_SQUARE); 

  if ( err != CL_SUCCESS )
  {
  	printf(" Erreur clSetKernelArg 1\n");
	exit(1);
  }	
  printf("Definition des arguments du Kernel 1 done\n");
  
  /* ==== globalsize and localsize for kernel 1=== */
  cl_event event_kernel1;
  size_t globalSize1[]={Np};
  size_t localSize1[]={64}; /* optimal value by default */
  localSize1[0]=atoi(argv[2]); /* choice of the local size by the parameter */
  printf("kernel1 : globalSize1= %lu\n",globalSize1[0]);
  printf("kernel1 : localSize1 = %lu\n",localSize1[0]);
  err = clEnqueueNDRangeKernel(queue,kernel1,1,NULL,globalSize1,localSize1,0,NULL,&event_kernel1);
  if ( err != CL_SUCCESS )
  {
  	printf(" Erreur Enqueue Kernel 1\n");
	exit(1);
  }	
  printf("Enqueue Kernel1\n");
  
