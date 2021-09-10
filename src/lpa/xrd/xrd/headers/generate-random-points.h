size_t irand=sizeof(cl_double)*Np;
  printf("irand    = %10lu  Bytes\n",irand);
  printf("irand    = %10lu kBytes\n",irand/1024);
  printf("dim irand --- Angle and Distance = %d\n",Np*2);

  printf("Allocate random numbers ... start\n");
  /* Initialize C random number generator */
  srand(time(NULL));
  /* Initialize Mersenne Twister random number generator */
  sgenrand(4357);
  RandRadius=(cl_double *)malloc(irand);
  //#define UNIFORM
  if ( UNIFORM == 0 )
  {
    printf("Uniform distribution on the whole disk\n"); 
    if ( RandMT == 0 )
    {
      printf("Using standard C random number generator\n");
      for ( i = 0 ; i < Np ; i++ )
        RandRadius[i]=sqrt((double)(((double) rand() / RAND_MAX)));
    }
    else
    {
      printf("Using Mersenne Twister random number generator implemented in C\n");
      for ( i = 0 ; i < Np ; i++ )
        RandRadius[i]=sqrt((double)(genrand()));
    }

  }
  else
  {
    printf("Uniform distribution near the center on the disk\n");
    if ( RandMT == 0 )
    {
      printf("Using standard C random number generator\n");
      for ( i = 0 ; i < Np ; i++ )
        RandRadius[i]=(double)(((double) rand() / RAND_MAX));
    }
    else
    {
      printf("Using Mersenne Twister random number generator implemented in C\n");
      for ( i = 0 ; i < Np ; i++ )
        RandRadius[i]=(double)(genrand());
    }
  }    
  RandAngle=(cl_double *)malloc(irand);
  if ( RandMT == 0 )
  {
    printf("Using standard C random number generator\n");
    for ( i = 0 ; i < Np ; i++ )
      RandAngle[i]=(double)(((double) rand() / RAND_MAX));
  }
  else
  {
    printf("Using Mersenne Twister random number generator implemented in C\n");
    for ( i = 0 ; i < Np ; i++ )
      RandAngle[i]=(double)(genrand());
  }
  printf("Allocate random numbers ... done\n");

  /*** may 2021 ***/
  printf("....create RandX random number array\n");
  RandX=(cl_double *)malloc(irand);
  if ( RandMT == 0 )
  {
    printf("Using standard C random number generator\n");
    for ( i = 0 ; i < Np ; i++ )
      RandX[i]=(double)(((double) rand() / RAND_MAX));
  }
  else
  {
    printf("Using Mersenne Twister random number generator implemented in C\n");
    for ( i = 0 ; i < Np ; i++ )
      RandX[i]=(double)(genrand());
  }
  printf("....create RandY random number array\n");
  RandY=(cl_double *)malloc(irand);
  if ( RandMT == 0 )
  {
    printf("Using standard C random number generator\n");
    for ( i = 0 ; i < Np ; i++ )
      RandY[i]=(double)(((double) rand() / RAND_MAX));
  }
  else
  {
    printf("Using Mersenne Twister random number generator implemented in C\n");
    for ( i = 0 ; i < Np ; i++ )
      RandY[i]=(double)(genrand());
  }

