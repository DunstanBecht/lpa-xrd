 /*
    ==================== Compute Fourier Coefficients by a scalar loop ==
    ==================== gpu implementation to be tuned later : 5 september 2018
    ==================== Dump FC to File indexed by the name
    */
    double time_reduction=wtime();

    /* === compute the total number of useful points === */
    cl_int Np_good=Np;
    for(i=0; i < Np; i++)
      Np_good -=(h_inout[i]==0)? 1:0;
    printf("Np      = %ld\n",Np);
    printf("Np-good = %ld\n",Np_good);

    double cAL=0.0f, cAL2=0.0f, sAL=0.0f, sAL2=0.0f, c2AL=0.0f, c2AL2=0.0f, s2AL=0.0f, s2AL2=0.0f, eps2=0.0f;
    double c3AL=0.0f, c3AL2=0.0f, s4AL=0.0f, s4AL2=0.0f, s5AL=0.0f, s5AL2=0.0f;
    double c4AL=0.0f, c4AL2=0.0f;
    double c5AL=0.0f, c5AL2=0.0f;
    double s3AL=0.0f, s3AL2=0.0f;


    /* compute by a reduction operation the errors for several Fourier Harmonic */
    for (i=0; i< Np; i++)
    {
      //printf("cAL[%d]= %lf\n",i,h_Vect8FC[i].s0);
      cAL +=h_Vect8FC[i].s0; cAL2 +=h_Vect8FC[i].s4;
      sAL +=h_Vect8FC[i].s1; sAL2 +=h_Vect8FC[i].s5;
      c2AL+=h_Vect8FC[i].s2; c2AL2+=h_Vect8FC[i].s6;
      s2AL+=h_Vect8FC[i].s3; s2AL2+=h_Vect8FC[i].s7;
      eps2+=h_Vect1FC[i];

      c3AL+=h_Vect16FC[i].s0; c3AL2+=h_Vect16FC[i].s2;
      s3AL+=h_Vect16FC[i].s1; s3AL2+=h_Vect16FC[i].s3;

      c4AL+=h_Vect16FC[i].s4; c4AL2+=h_Vect16FC[i].s6;
      s4AL+=h_Vect16FC[i].s5; s4AL2+=h_Vect16FC[i].s7;

      c5AL+=h_Vect16FC[i].s8; c5AL2+=h_Vect16FC[i].sa;
      s5AL+=h_Vect16FC[i].s9; s5AL2+=h_Vect16FC[i].sb;

    }
//cAL,sAL,c2AL,s2AL,cAL2,sAL2,c2AL2,s2AL2
    time_reduction=wtime()-time_reduction;
    printf("\n--------\ntime-reduction [s]= %e\n",time_reduction);
    double coeff=1.0f/(double)(Np_good);
    printf("coeff = 1/Np= %lf\n",coeff);
    double factor=(double)coeff; /* 2 septembre 2019 */
    cAL*=coeff	; cAL2*=coeff; sAL*=coeff; sAL2*=coeff; c2AL*=coeff; c2AL2*=coeff; s2AL*=coeff; s2AL2*=coeff;
    c3AL*=coeff ; c3AL2*=coeff; s3AL*=coeff ; s3AL2*=coeff;
    c4AL*=coeff ; c4AL2*=coeff; s4AL*=coeff ; s4AL2*=coeff;
    c5AL*=coeff ; c5AL2*=coeff; s5AL*=coeff ; s5AL2*=coeff;

    eps2*=coeff;

    /* valeur de la moyene */
    double mean_cAL=cAL;
    double mean_sAL=sAL;
    double mean_c2AL=c2AL;
    double mean_s2AL=s2AL;

    double mean_c3AL=c3AL;
    double mean_s3AL=s3AL;

    double mean_c4AL=c4AL;
    double mean_s4AL=s4AL;

    double mean_c5AL=c5AL;
    double mean_s5AL=s5AL;

    double err_C=0.0f;
    double err_S=0.0f;
    double err_C2=0.0f;
    double err_S2=0.0f;
    double err_C3=0.0f;
    double err_S3=0.0f;
    double err_C4=0.0f;
    double err_S4=0.0f;
    double err_C5=0.0f;
    double err_S5=0.0f;

    for (i=0; i< Np; i++)
    {
      if (h_inout[i]==1)
      {
        err_C +=(h_Vect8FC[i].s0-mean_cAL )*(h_Vect8FC[i].s0-mean_cAL );
        err_S +=(h_Vect8FC[i].s1-mean_sAL )*(h_Vect8FC[i].s1-mean_sAL );
        err_C2+=(h_Vect8FC[i].s2-mean_c2AL)*(h_Vect8FC[i].s2-mean_c2AL);
        err_S2+=(h_Vect8FC[i].s3-mean_s2AL)*(h_Vect8FC[i].s3-mean_s2AL);

        err_C3+=(h_Vect16FC[i].s0-mean_c3AL)*(h_Vect16FC[i].s0-mean_c3AL);
        err_S3+=(h_Vect16FC[i].s1-mean_s3AL)*(h_Vect16FC[i].s1-mean_s3AL);

        err_C4+=(h_Vect16FC[i].s4-mean_c4AL)*(h_Vect16FC[i].s4-mean_c4AL);
        err_S4+=(h_Vect16FC[i].s5-mean_s4AL)*(h_Vect16FC[i].s5-mean_s4AL);

        err_C5+=(h_Vect16FC[i].s8-mean_c5AL)*(h_Vect16FC[i].s8-mean_c5AL);
        err_S5+=(h_Vect16FC[i].s9-mean_s5AL)*(h_Vect16FC[i].s9-mean_s5AL);

      }
    }
   // Avant mars 2019 c'est une erreur ! :-)
    err_C =sqrt(err_C*coeff);
    err_S =sqrt(err_S*coeff);
    err_C2=sqrt(err_C2*coeff);
    err_S2=sqrt(err_S2*coeff);
   // Correction 13 mars 2019
    err_C =sqrt(err_C)*coeff;
    err_S =sqrt(err_S)*coeff;
    err_C2=sqrt(err_C2)*coeff;
    err_S2=sqrt(err_S2)*coeff;

    err_C3=sqrt(err_C3)*coeff;
    err_S3=sqrt(err_S3)*coeff;

    err_C4=sqrt(err_C4)*coeff;
    err_S4=sqrt(err_S4)*coeff;

    err_C5=sqrt(err_C5)*coeff;
    err_S5=sqrt(err_S5)*coeff;


//    double err_C =sqrt((cAL2-cAL*cAL)*coeff);
//    double err_S =sqrt((sAL2-sAL*sAL)*coeff);
//    double err_C2=sqrt((c2AL2-c2AL*c2AL)*coeff);
//    double err_S2=sqrt((s2AL2-s2AL*s2AL)*coeff);
    printf("cAL2 =%lf cAL =%lf cAL *cAL =%lf err_C =%lf\n",cAL2,cAL,cAL*cAL,err_C);
    printf("sAL2 =%lf sAL =%lf sAL *sAL =%lf err_S =%lf\n",sAL2,sAL,sAL*sAL,err_S);
    printf("c2AL2=%lf c2AL=%lf c2AL*c2AL=%lf err_C2=%lf\n",c2AL2,c2AL,c2AL*c2AL,err_C2);
    printf("s2AL2=%lf s2AL=%lf s2AL*s2AL=%lf err_S2=%lf\n",s2AL2,s2AL,s2AL*s2AL,err_S2);
    printf("c3AL2=%lf c3AL=%lf c3AL*c3AL=%lf err_C3=%lf\n",c3AL2,c3AL,c3AL*c3AL,err_C3);
    printf("s3AL2=%lf s3AL=%lf s3AL*s3AL=%lf err_S3=%lf\n",s3AL2,s3AL,s3AL*s3AL,err_S3);
    printf("c4AL2=%lf c4AL=%lf c4AL*c4AL=%lf err_C4=%lf\n",c4AL2,c4AL,c4AL*c4AL,err_C4);
    printf("s4AL2=%lf s4AL=%lf s4AL*s4AL=%lf err_S4=%lf\n",s4AL2,s4AL,s4AL*s4AL,err_S4);
    printf("c5AL2=%lf c5AL=%lf c5AL*c5AL=%lf err_C5=%lf\n",c5AL2,c5AL,c5AL*c5AL,err_C5);
    printf("s5AL2=%lf s5AL=%lf s5AL*s5AL=%lf err_S5=%lf\n",s5AL2,s5AL,s5AL*s5AL,err_S5);

    /*== Dump to File the Fourier coefficients ==*/
    fprintf(FileFC,"%6.1lf\t%10.7f\t%10.7f\t%10.7f\t%10.7f\t%10.7f\t%10.7f\t%10.7f\t%10.7f\t%10.7f\t%10.7f\t%10.7f\t%10.7f\t%10.7f\t%10.7f\t%10.7f\t%10.7f\t%10.7f\t%10.7f\t%10.7f\t%10.7f\t%10.7f\t%4d\n",
            IndexFourier*a3,
            cAL, err_C, sAL, err_S,
            c2AL, err_C2, s2AL, err_S2,
            c3AL, err_C3, s3AL, err_S3,
            c4AL, err_C4, s4AL, err_S4,
            c5AL, err_C5, s5AL, err_S5,
            eps2, Np-Np_good);
  }
  printf("...\nEnd computations on the gpu \n");

  /* ==== Dump results to file for analysis === */
  if ( DUMP == 1)
  {
  FILE *fr1;
  char Namer1[]="r1_";
  fr1=fopen("r1.res","w");
  for ( i = 0 ; i < Np ; i++ )
    fprintf(fr1,"%lf %lf\n",h_r1[i].x,h_r1[i].y);
  fclose(fr1);
  printf("--Dump 2d field r1(x,y) \n");


  FILE *fu1;
  fu1=fopen("u1.res","w");
  for ( i = 0 ; i < Np ; i++ )
    fprintf(fu1,"%lf %lf %lf\n",h_u1[i].x,h_u1[i].y,h_u1[i].z);
  fclose(fu1);
  printf("--Dump 3d field u1(x,y,z) \n");
  }
