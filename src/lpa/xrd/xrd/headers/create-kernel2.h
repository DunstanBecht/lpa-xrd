
  /*
  ------------ Creation du second kernel : comptf
  */
  printf("  Start Kernel 2 creation and analysis \n");  
  /*----------------------------------------
   *
   * Liste des variables sur le device : cpu 
   *
  -----------------------------------------*/
  /* Allocation dynamique                    */
  cl_double16 *h_Vect16FC;
  printf(" size of cl_double16 = %lu\n",sizeof(cl_double16));
  size_t  bytes_Vect16FC= sizeof(cl_double16)*Np;
  h_Vect16FC = (cl_double16 *)malloc(bytes_Vect16FC);
  if ( h_Vect16FC == NULL )
  {
    printf("Erreur allocation mem h_Vect16FC\n");
  }
  printf("Allocation host : h_Vect16FC de %lu  Bytes\n",bytes_Vect16FC);
  printf("Allocation host : h_Vect16FC de %lu kBytes\n",bytes_Vect16FC/1024);
  printf("Initialize to zero the Vect16FC \n");
  int mm;
  for ( mm=0; mm < Np; mm++)
  {
   h_Vect16FC[mm].s0=0.0f;
   h_Vect16FC[mm].s1=0.0f;
   h_Vect16FC[mm].s2=0.0f;
   h_Vect16FC[mm].s3=0.0f;
   h_Vect16FC[mm].s4=0.0f;
   h_Vect16FC[mm].s5=0.0f;
   h_Vect16FC[mm].s6=0.0f;
   h_Vect16FC[mm].s7=0.0f;
   h_Vect16FC[mm].s8=0.0f;
   h_Vect16FC[mm].s9=0.0f;
   h_Vect16FC[mm].sa=0.0f;
   h_Vect16FC[mm].sb=0.0f;
   h_Vect16FC[mm].sc=0.0f;
   h_Vect16FC[mm].sd=0.0f;
   h_Vect16FC[mm].se=0.0f;
   h_Vect16FC[mm].sf=0.0f;
  }

  cl_double8 *h_Vect8FC;
  printf(" size of cl_double8 = %lu\n",sizeof(cl_double8));
  size_t  bytes_Vect8FC= sizeof(cl_double8)*Np;
  h_Vect8FC = (cl_double8 *)malloc(bytes_Vect8FC);
  if ( h_Vect8FC == NULL )
  {
    printf("Erreur allocation mem h_Vect8FC\n");
  }
  printf("Allocation host : h_Vect8FC de %lu  Bytes\n",bytes_Vect8FC);
  printf("Allocation host : h_Vect8FC de %lu kBytes\n",bytes_Vect8FC/1024);
  printf("Initialize to zero the Vect8FC \n");
  for ( mm=0; mm < Np; mm++)
  {
   h_Vect8FC[mm].s0=0.0f;
   h_Vect8FC[mm].s1=0.0f;
   h_Vect8FC[mm].s2=0.0f;
   h_Vect8FC[mm].s3=0.0f;
   h_Vect8FC[mm].s4=0.0f;
   h_Vect8FC[mm].s5=0.0f;
   h_Vect8FC[mm].s6=0.0f;
   h_Vect8FC[mm].s7=0.0f;
  }

  cl_double *h_Vect1FC;
  size_t  bytes_Vect1FC= sizeof(cl_double)*Np;
  h_Vect1FC = (cl_double *)malloc(bytes_Vect1FC);
  if ( h_Vect1FC == NULL )
  {
    printf("Erreur allocation mem h_Vect1FC\n");
  }
  for ( mm=0; mm < Np; mm++)
    h_Vect1FC[mm]=0.0f;
  
  printf("Allocation host : h_Vect1FC de %lu  Bytes\n",bytes_Vect1FC);
  printf("Allocation host : h_Vect1FC de %lu kBytes\n",bytes_Vect1FC/1024);
  
  size_t  bytes_inout= sizeof(cl_int)*Np;
  cl_int *h_inout;
  h_inout = (cl_int *)malloc(bytes_inout);
  /* tableau d'entiers : 0 ou 1 : point inside or outside */
  if ( h_inout == NULL )
  {
    printf("Erreur allocation mem h_inout\n");
  }
  for ( mm=0; mm < Np; mm++) h_inout[mm]=0;

  /*----------------------------------------
   *
   * Liste des variables sur le device : gpu 
   *
  -----------------------------------------*/
  cl_mem   d_Vect16FC=clCreateBuffer(context,CL_MEM_READ_WRITE|CL_MEM_ALLOC_HOST_PTR,bytes_Vect16FC,NULL,NULL);
  cl_mem   d_Vect8FC=clCreateBuffer(context,CL_MEM_READ_WRITE|CL_MEM_ALLOC_HOST_PTR,bytes_Vect8FC,NULL,NULL);
  cl_mem   d_Vect1FC=clCreateBuffer(context,CL_MEM_READ_WRITE|CL_MEM_ALLOC_HOST_PTR,bytes_Vect1FC,NULL,NULL);
  cl_mem   d_inout=clCreateBuffer(context,CL_MEM_READ_WRITE|CL_MEM_ALLOC_HOST_PTR,bytes_inout,NULL,NULL);
  printf("---create buffer on the gpu---\n"); 
  /*----------------------------------------
   *
   * Copie des donnees du host(cpu) vers le device(gpu)
   * pour le kernel 2 
   * --> Ce sont ds tableaux monodiensionnels de type F8 ou F1 que l'on alloue sur le gpu
  -----------------------------------------*/  
  err  = clEnqueueWriteBuffer(queue,d_Vect16FC,CL_TRUE,0,bytes_Vect16FC,h_Vect16FC,0,NULL,NULL);
  err |= clEnqueueWriteBuffer(queue,d_Vect8FC,CL_TRUE,0,bytes_Vect8FC,h_Vect8FC,0,NULL,NULL);
  err |= clEnqueueWriteBuffer(queue,d_Vect1FC,CL_TRUE,0,bytes_Vect1FC,h_Vect1FC,0,NULL,NULL);
  err |= clEnqueueWriteBuffer(queue,d_inout,CL_TRUE,0,bytes_inout,h_inout,0,NULL,NULL);
  if ( err != CL_SUCCESS )
  {
  	printf(" Erreur Memory kernel2 \n");
  }	

  /*----------------------------------------
   *
   * Creation du kernel 2
   *
  -----------------------------------------*/
  
  cl_kernel kernel2 = clCreateKernel(program,"comptf", &err);
   if ( err != CL_SUCCESS )
  {
  	printf(" Erreur creation kernel 2\n");
	exit(1);
  }	

  printf("Creation du Kernel 2 :-- Compute Fourier Transform coefficients\n");
  
  /* ==== command queue is defined === */
  clFinish(queue);
  printf("clFinish(queue);\n");
