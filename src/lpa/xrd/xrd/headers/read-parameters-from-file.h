/*
  -------------------------
  Read Parameters from file
  -------------------------
  */
  FILE *fbe;
  char buff[BUZZ_SIZE];
  int idmod;
  size_t i , j, k , p , q;

  /* -- list of variables of the computation -- */
  cl_double3 ld; 
  cl_double3 ex;
  cl_double3 ey;
  cl_double3 bd;/* Burgers vector */
  cl_double3 ez;
  cl_double3 H; /* Miller indices of the diffracting planes hkl */
  cl_double3 eg;
  cl_double  a_cell_param;
  cl_double  cfact_str;
  cl_double  bsd;
  cl_double3 bscrew;
  cl_double3 bedge;
  cl_double  bed;
  cl_double3 egd;
  cl_double3 gd;
  cl_double3 a3v;
  cl_double2 a3vd;
  cl_double  Dx;
  cl_double  Dy;
  cl_int    Np;
  cl_double  a3;
  cl_int    NoFC;
  cl_int    BCtype;
  cl_double  nu;
  cl_int    Nd0;
  cl_double2 *rd0;
  cl_double2 *rd0_all;
  cl_int    *sd0;
  cl_int    *sd0_all;
  cl_int    Nd;
  cl_double  *be;
  cl_double  *bs;
  cl_double  uaux;
  cl_double  uaux1;
  cl_double  uaux2;
  cl_double  *RandRadius; /* random numbers for radius in polar coordinates */
  cl_double  *RandAngle; /* random numbers for angle in polar coordinates */
  cl_double  *RandX; /* random numbers for X in cartesian coordinates */
  cl_double  *RandY; /* random numbers for Y in cartesian coordinates */
  cl_double  Radius;  /* radius of the dislocation points */
  cl_double8 *FC;  /* Pack 8 fourier coefficients */
  
  cl_int    IndexFourier; /* entier entre 0 et NoFC-1, pour le calcul de la distance */  

  /* -- list of variables of the computation -- */

  printf("\n");
  Np   =atoi(argv[4]); printf("Np  = %ld\n",Np);
  NoFC =atoi(argv[5]); printf("NoFC= %ld\n",NoFC);
  printf("+++---Cylindrical----+++++++\n");
  printf("++++++++++++++++++++++++++++\n");
  printf("Start Olvas_model function\n");

  printf("Parameter File = %s\n",argv[3]);
  fbe=fopen(argv[3],"r");
  if (fbe==NULL)
  {
    printf("\n----Error reading file : %s\n",argv[3]);
    exit(1);
  }
 
  fgets(buff, BUZZ_SIZE, fbe); 
  fscanf(fbe,"%lf %lf %lf",&ld.x,&ld.y,&ld.z);
  printf("Normal of the model plane parallel to  l \n");
  printf("ld.x=%lf\n",ld.x);
  printf("ld.y=%lf\n",ld.y);
  printf("ld.z=%lf\n",ld.z);
  printf("\n");

  fgets(buff, BUZZ_SIZE, fbe); 
  fscanf(fbe,"%lf %lf %lf",&ex.x,&ex.y,&ex.z);
  printf("Crystallographic direction along x \n");
  printf("ex.x=%lf\n",ex.x);
  printf("ex.y=%lf\n",ex.y);
  printf("ex.z=%lf\n",ex.z);
  printf("\n");

  fgets(buff, BUZZ_SIZE, fbe); 
  fscanf(fbe,"%lf %lf %lf",&bd.x,&bd.y,&bd.z);
  printf("Burgers vector \n");
  printf("bd.x=%lf\n",bd.x);
  printf("bd.y=%lf\n",bd.y);
  printf("bd.z=%lf\n",bd.z);
  printf("\n");

  ez.x=ld.x;
  ez.y=ld.y;
  ez.z=ld.z;
  /* Normalize ez */
  double ezl=sqrt(ez.x*ez.x+ez.y*ez.y+ez.z*ez.z);
  ez.x/=ezl;
  ez.y/=ezl;
  ez.z/=ezl;
  printf("ez.x=%lf\n",ez.x);
  printf("ez.y=%lf\n",ez.y);
  printf("ez.z=%lf\n",ez.z);
  printf("\n");

  fgets(buff, BUZZ_SIZE, fbe); 
  fscanf(fbe,"%lf %lf %lf",&H.x,&H.y,&H.z);
  printf("Diffraction vector hkl \n");
  printf("H.x=%lf\n",H.x);
  printf("H.y=%lf\n",H.y);
  printf("H.z=%lf\n",H.z);
  printf("\n");

  eg.x=H.x;
  eg.y=H.y;
  eg.z=H.z;
  double egl=sqrt(eg.x*eg.x+eg.y*eg.y+eg.z*eg.z);
  eg.x/=egl;
  eg.y/=egl;
  eg.z/=egl;
  /* Normalize eg */
  printf("eg.x=%lf\n",eg.x);
  printf("eg.y=%lf\n",eg.y);
  printf("eg.z=%lf\n",eg.z);
  printf("\n");

  fgets(buff, BUZZ_SIZE, fbe); 
  fscanf(fbe,"%lf",&cfact_str);
  printf("cfact_str=%lf\n",cfact_str);
  printf("\n");
  
  fgets(buff, BUZZ_SIZE, fbe); 
  fscanf(fbe,"%lf",&a_cell_param);
  printf("cell parameter a  =%lf\n",a_cell_param);
  printf("\n");
  
  /* bd=a/2*bd */ 
  bd.x *=a_cell_param/2;
  bd.y *=a_cell_param/2;
  bd.z *=a_cell_param/2;
  printf("bd.x=%lf\n",bd.x);
  printf("bd.y=%lf\n",bd.y);
  printf("bd.z=%lf\n",bd.z);
  printf("\n");

  /* bsd= ez.bd */
  bsd=ez.x*bd.x+ez.y*bd.y+ez.z*bd.z;
  printf("bsd =%lf\n",bsd);
  printf("\n");

  /* bscrew=bsd * ez */
  bscrew.x=bsd*ez.x;
  bscrew.y=bsd*ez.y;
  bscrew.z=bsd*ez.z;

  printf("bscrew.x=%lf\n",bscrew.x);
  printf("bscrew.y=%lf\n",bscrew.y);
  printf("bscrew.z=%lf\n",bscrew.z);
  printf("\n");

  /* bedge=bd-bscrew */
  bedge.x=bd.x-bscrew.x;
  bedge.y=bd.y-bscrew.y;
  bedge.z=bd.z-bscrew.z;
  printf("bedge.x=%lf\n",bedge.x);
  printf("bedge.y=%lf\n",bedge.y);
  printf("bedge.z=%lf\n",bedge.z);
  printf("\n");

  bed=length3(bedge); /* Nagysag3 */
  printf("bed =%lf\n",bed);
  printf("\n");

  /* lenght bscrew */
  cl_double bscrewl=length3(bscrew);
  printf("norm bscrew = %lf\n",bscrewl);
  printf("\n");
 
  if ( bscrewl < EPS )
  {
    printf("  edge   dislocation\n");
    bed=length3(bd);
    bsd=0.0;
  } 
  else if (length3(bedge) < EPS )
  {
    printf("  screw  dislocation\n");
    bsd=length3(bd);
    bed=0.0;
  }
  else
  {
    printf("  mixed dislocation\n");
    ex.x=bedge.x;
    ex.y=bedge.y;
    ex.z=bedge.z;
  }

  /* Normalize ex */
  cl_double exl=length3(ex);
  ex.x/=exl;
  ex.y/=exl;
  ex.z/=exl;
  printf("exl  = %lf\n",exl);
  printf("ex.x = %lf\n",ex.x);
  printf("ex.y = %lf\n",ex.y);
  printf("ex.z = %lf\n",ex.z);
 
  
  /* vector product ey=ez x ex */
  ey.x =ez.y*ex.z-ez.z*ex.y;
  ey.y =ez.z*ex.x-ez.x*ex.z;
  ey.z =ez.x*ex.y-ez.y*ex.x;

  /* ---- 1 : create the 3x3 matrix    */  
  cl_double Om[3][3];
  Om[0][0]=ex.x;
  Om[0][1]=ey.x;
  Om[0][2]=ez.x;
  
  Om[1][0]=ex.y;
  Om[1][1]=ey.y;
  Om[1][2]=ez.y;

  Om[2][0]=ex.z;
  Om[2][1]=ey.z;
  Om[2][2]=ez.z;


  Om[0][0]=ex.x;
  Om[0][1]=ex.y;
  Om[0][2]=ex.z;
  
  Om[1][0]=ey.x;
  Om[1][1]=ey.y;
  Om[1][2]=ey.z;

  Om[2][0]=ez.x;
  Om[2][1]=ez.y;
  Om[2][2]=ez.z;

  
  /* ---- 2     */
  cl_double coeff=length3(H)/a_cell_param;
  /* ---- 2 bis g= coeff * eg*/
  cl_double3 g;
  g.x=coeff*eg.x;
  g.y=coeff*eg.y;
  g.z=coeff*eg.z;

  /* ---- 3 : product matrix vector Om by g:   gd=Om g*/ 
  gd.x=Om[0][0]*g.x+Om[0][1]*g.y+Om[0][2]*g.z;
  gd.y=Om[1][0]*g.x+Om[1][1]*g.y+Om[1][2]*g.z;
  gd.z=Om[2][0]*g.x+Om[2][1]*g.y+Om[2][2]*g.z;

  printf("om00=%lf om01=%lf om02=%lf\n",Om[0][0],Om[0][1],Om[0][2]);
  printf("om10=%lf om11=%lf om11=%lf\n",Om[1][0],Om[1][1],Om[1][2]);
  printf("om20=%lf om21=%lf om21=%lf\n",Om[2][0],Om[2][1],Om[2][2]);
  

  printf("gd.x = %lf\n",gd.x);
  printf("gd.y = %lf\n",gd.y);
  printf("gd.z = %lf\n",gd.z);
  
  /* ---- 4 : egd=gd */
  egd.x=gd.x;
  egd.y=gd.y;
  egd.z=gd.z;
  /* ---- 5 : Normalize egd */
  cl_double norm=length3(egd);
  egd.x/=norm;
  egd.y/=norm;
  egd.z/=norm;
  
  printf("...may 2021 read the flag parameter Cylinder or Square_\n");
  /* ---- */
  fgets(buff, BUZZ_SIZE, fbe); 
  fscanf(fbe,"%lf",&Radius);
  printf("Radius  =%lf\n",Radius);
  printf("\n");

  fgets(buff, BUZZ_SIZE, fbe); 
  printf("string read from file = %s\n",buff);

  char str_cylinder[12] = "Cylinder";
  char str_square[12] = "Square";
  
  FLAG_CYLINDER=2;
  FLAG_SQUARE=2;

  char *resucyl = strstr(buff,"Cylinder");
  if ( resucyl != NULL )
  {
    printf("Mot Clef Cylinder OK\n");
    FLAG_CYLINDER=1;
    FLAG_SQUARE=0;
  }
  
  char *resusquare = strstr(buff,"Square");
  if ( resusquare != NULL )
  {
    printf("Mot Clef Square OK\n");
    FLAG_CYLINDER=0;
    FLAG_SQUARE=1;

    /*--get the value of the number of replications */
    char numbers[10];
    strcat(numbers,"");
    int i=0; 
    printf("longueur=%ld\n",strlen(buff));
    int index=0;
    for (i = 0; i<strlen(buff); i++)
    { 
      printf("i=%d\n",i);
      if (isdigit(buff[i])) 
      {
       //printf("ok caractere=int %d\n",buff[i]);
       numbers[index]=buff[i];
       index+=1;
      }
    }
    //printf("chaine= %s\n",numbers);
    int entier=atoi(numbers);
    printf("replication D=%d\n",entier);
    D_REPLICATION=entier;
  }
  
  fscanf(fbe,"%lf",&a3);
  printf("a3  =%lf\n",a3);
  printf("\n");

  /* ---- 6     */
  a3v.x=a3*egd.x;
  a3v.y=a3*egd.y;
  a3v.z=a3*egd.z;

  /* ---- 7     */
  a3vd.x=a3v.x;
  a3vd.y=a3v.y;
  printf("a3vd.x= %lf\n",a3vd.x);
  printf("a3vd.y= %lf\n",a3vd.y);
  
  fgets(buff, BUZZ_SIZE, fbe); 
  fscanf(fbe,"%lf",&nu);
  printf("nu  =%lf\n",nu);
  printf("\n");

  fgets(buff, BUZZ_SIZE, fbe); 
  fscanf(fbe,"%d",&Nd0);
  printf("Nd0 =%d\n",Nd0);
  printf("\n");

  size_t ird0=sizeof(cl_double2)*Nd0;
  printf("ird0= %10lu  Bytes\n",ird0);
  printf("ird0= %10lu kBytes\n",ird0/1024);

  /* -- allocate vector -- */  
  rd0 = (cl_double2 *)malloc(sizeof(cl_double2)*Nd0);
  
  size_t isd0=sizeof(cl_int)*Nd0;
  printf("isd0= %10lu  Bytes\n",isd0);
  printf("isd0= %10lu kBytes\n",isd0/1024);

  sd0 = (cl_int *)malloc(sizeof(cl_int)*Nd0);

  fgets(buff, BUZZ_SIZE, fbe); 
  fgets(buff, BUZZ_SIZE, fbe); 
  /*  sd0: sign of dislocation 
      rd0: abscissa of dislocation
      rd1: y of dislocation
  */
  FILE *pdislo; pdislo=fopen("pdislo.res","w");
  FILE *ndislo; ndislo=fopen("ndislo.res","w");
  for(i=0 ; i < Nd0 ; i++ )
  {
    fscanf(fbe,"%d %le %le\n",&sd0[i],&rd0[i].x,&rd0[i].y);
    printf("\ni   = %6ld\n",i);
    printf("sd0  = %d\n",sd0[i]);
    printf("rd0.x= %lf\n",rd0[i].x);
    printf("rd0.y= %lf\n",rd0[i].y);
    if ( sd0[i] < 0 )
      fprintf(ndislo,"%e %e\n",rd0[i].x,rd0[i].y);
    else
      fprintf(pdislo,"%e %e\n",rd0[i].x,rd0[i].y);
  } 
  fclose(fbe);
  fclose(ndislo);
  fclose(pdislo);
  printf("++++++++++++++++++++++++++++\n");
  printf("\n");
  printf("Generate dislocations with replications if Square option is defined...\n");
  Nd=Nd0;
  /** may 2021 **/
  if ( FLAG_SQUARE== 1)
  {
    printf("Number of replications                         = %d\n",D_REPLICATION);
    printf("Number of dislocations in the elementary cell  = %d\n",Nd0);
    Nd=Nd0*(2*D_REPLICATION+1)*(2*D_REPLICATION+1);
    printf("Number of dislocations in all replicated cells = %d\n",Nd);
  }
