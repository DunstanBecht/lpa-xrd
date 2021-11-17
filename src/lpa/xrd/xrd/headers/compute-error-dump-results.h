    /*
     * Compute The value of the Fourier coefficients and the quadratic error
    */
    double time_reduction=wtime();

    /* === compute the total number of useful points === */
    cl_int Np_good=Np;
    for(i=0; i < Np; i++)
      Np_good -=(h_inout[i]==0)? 1:0;
    printf("Np      = %ld\n",Np);
    printf("Np-good = %ld\n",Np_good);

    double eps2=0.0f;
    double cAL=0.0f, c2AL=0.0f, c3AL=0.0f, c4AL=0.0f, c5AL=0.0f;
    double sAL=0.0f, s2AL=0.0f, s3AL=0.0f, s4AL=0.0f, s5AL=0.0f;

    /* compute by a reduction operation the errors for several Fourier Harmonic */
    for (i=0; i< Np; i++)
    {
      // error coefficient
      eps2+=h_Vect16FC[i].sa;
      // For each harmonic compute the real and imaginary parts and
      cAL +=h_Vect16FC[i].s0; sAL +=h_Vect16FC[i].s1; 
      c2AL+=h_Vect16FC[i].s2; s2AL+=h_Vect16FC[i].s3; 
      c3AL+=h_Vect16FC[i].s4; s3AL+=h_Vect16FC[i].s5; 
      c4AL+=h_Vect16FC[i].s6; s4AL+=h_Vect16FC[i].s7; 
      c5AL+=h_Vect16FC[i].s8; s5AL+=h_Vect16FC[i].s9; 
    }
    time_reduction=wtime()-time_reduction;
    printf("\n--------\ntime-reduction [s]= %e\n",time_reduction);
    double coeff=1.0f/(double)(Np_good);
    printf("coeff = 1/Np= %lf\n",coeff);
    double factor=(double)coeff; /* 2 septembre 2019 */
    cAL *=coeff; sAL *=coeff; 
    c2AL*=coeff; s2AL*=coeff; 
    c3AL*=coeff; s3AL*=coeff;
    c4AL*=coeff; s4AL*=coeff; 
    c5AL*=coeff; s5AL*=coeff; 

    eps2*=coeff;

    /* mean values of the harmonics */
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
        err_C +=(h_Vect16FC[i].s0-mean_cAL )*(h_Vect16FC[i].s0-mean_cAL );
        err_S +=(h_Vect16FC[i].s1-mean_sAL )*(h_Vect16FC[i].s1-mean_sAL );

        err_C2+=(h_Vect16FC[i].s2-mean_c2AL)*(h_Vect16FC[i].s2-mean_c2AL);
        err_S2+=(h_Vect16FC[i].s3-mean_s2AL)*(h_Vect16FC[i].s3-mean_s2AL);

        err_C3+=(h_Vect16FC[i].s4-mean_c3AL)*(h_Vect16FC[i].s4-mean_c3AL);
        err_S3+=(h_Vect16FC[i].s5-mean_s3AL)*(h_Vect16FC[i].s5-mean_s3AL);

        err_C4+=(h_Vect16FC[i].s6-mean_c4AL)*(h_Vect16FC[i].s6-mean_c4AL);
        err_S4+=(h_Vect16FC[i].s7-mean_s4AL)*(h_Vect16FC[i].s7-mean_s4AL);

        err_C5+=(h_Vect16FC[i].s8-mean_c5AL)*(h_Vect16FC[i].s8-mean_c5AL);
        err_S5+=(h_Vect16FC[i].s9-mean_s5AL)*(h_Vect16FC[i].s9-mean_s5AL);
      }
    }
    // Compute the quadratic errors
    err_C =sqrt(err_C)*coeff; err_S =sqrt(err_S)*coeff;
    err_C2=sqrt(err_C2)*coeff; err_S2=sqrt(err_S2)*coeff;
    err_C3=sqrt(err_C3)*coeff; err_S3=sqrt(err_S3)*coeff;
    err_C4=sqrt(err_C4)*coeff; err_S4=sqrt(err_S4)*coeff;
    err_C5=sqrt(err_C5)*coeff; err_S5=sqrt(err_S5)*coeff;

    printf("c1AL=%lf c1AL*c1AL=%lf err_C =%lf\n",cAL,cAL*cAL,err_C);
    printf("s1AL=%lf s1AL*s1AL=%lf err_S =%lf\n",sAL,sAL*sAL,err_S);
    printf("c2AL=%lf c2AL*c2AL=%lf err_C2=%lf\n",c2AL,c2AL*c2AL,err_C2);
    printf("s2AL=%lf s2AL*s2AL=%lf err_S2=%lf\n",s2AL,s2AL*s2AL,err_S2);
    printf("c3AL=%lf c3AL*c3AL=%lf err_C3=%lf\n",c3AL,c3AL*c3AL,err_C3);
    printf("s3AL=%lf s3AL*s3AL=%lf err_S3=%lf\n",s3AL,s3AL*s3AL,err_S3);
    printf("c4AL=%lf c4AL*c4AL=%lf err_C4=%lf\n",c4AL,c4AL*c4AL,err_C4);
    printf("s4AL=%lf s4AL*s4AL=%lf err_S4=%lf\n",s4AL,s4AL*s4AL,err_S4);
    printf("c5AL=%lf c5AL*c5AL=%lf err_C5=%lf\n",c5AL,c5AL*c5AL,err_C5);
    printf("s5AL=%lf s5AL*s5AL=%lf err_S5=%lf\n",s5AL,s5AL*s5AL,err_S5);

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
