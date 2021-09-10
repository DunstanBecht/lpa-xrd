/*
10 mai 2021-- autre version du code
*/
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
/* dpcaltfalldislo.cl */
/* 3 septembre 2019 Toutes les variables stockant les positions sont 
   en memoire globale: probleme cela necessite de faire beaucoup de transfert 
   entre les niveaux de hierarchie memoire
*/
/* version 15 mai 2019 */
/* Pour chaque point aleatoire on associe un entier qui vaut 0 ou 1  
   0 : le point n'est pas a l'interieur de la sphere
   1 : le point est a l'interieur de la sphere
   On sommera ensuite ces valeurs pour obtenir le nombre total de points
   qio ont été utilisés
   Ce nombre servira dans le calcul de moyenne
*/
/* 24 janvier 2019 : Correction de l'expression des coefficients uaux etc. */ 
__kernel void udislo(__global double *RandA,
                     __global double *RandB,
                     __global double2 *rd0, 
                     __global double2 *r1, 
                     __global double3 *u1, 
                     __global double *be, 
                     __global double *bs, 
                     const double Radius,
                     const double nu,
                     const int Np,
                     const int Nd,
		     const int Flag_Square)
/*  flag cylindiruqe ou cartesion */
/*  nb aleatoire unifiormement distribues sur carre */
{
  /* Index of the thread : i.e. number of the current element */
  /* thread between 0 and NoFC : index l = same as Andras pascal program  */
  int k = get_global_id(0); 
/* On peut avoir des millions de points aleatoires generes < int = 2 147 483 647 : amplement suffisant */
  u1[k]=(double3)(0.0f,0.0f,0.0f);
  r1[k]=(double2)(0.0f,0.0f);
  /* variables temporaires */
  double uaux =1.0f/(4.0f*(1.0f-nu));
  double uaux1=1.0f/(2.0f*(1-nu));
  double uaux2=1.0f-2.0f*nu;
  double pi=3.1415926535897f;
  //printf("kernel uaux = %f\n",uaux);
  //printf("kernel uaux1 = %f\n",uaux1);
  //printf("kernel uaux2 = %f\n",uaux2);
  /*---*/
  //printf("kernel Nd= %d\n",Nd);
  if ( k < Np )
  { 
    /*-- Define Points in a cartesian or cylindrical coordinate system --*/ 
    double2 r11=(double2)(0.0f,0.0f);
    if (Flag_Square == 1)
    {
    r11.x=RandA[k]*Radius;
    r11.y=RandB[k]*Radius;
    }
    else
    {
      double r=RandA[k]*Radius;
      double phi=RandB[k]*2.0f*pi;
      r11.x=r*cos(phi); 
      r11.y=r*sin(phi); 
    } 
    
    /*-- Compute Field Generated --*/ 
    /*--dupliquer le code de calcul du champ ---*/
    /*--On ne peut pas avec OpenCL 1.1 (C99) appeler une fonction qui a en param des pointeurs --*/
    double3 u2=(double3)(0.0f,0.0f,0.0f);
    double2 r0=(double2)(0.0f,0.0f);
    //#pragma unroll 
    double zzz=0.0f;
    double ax2x1;
    double x1;
    double x2;
    double x1x2;
    int kk;
    //#pragma unroll 1
    for (kk=0; kk < Nd; kk++)
    {
      /* r0= distance entre r2 et la kk-ime dislocation */
        r0=r11-rd0[kk];
      //printf("r0 kernel= %f %f %f\n",r0.x,r0.y);
      //if ( length(r0) > 0 )
      {  
        x1  =r0.x; x2  =r0.y; x1x2=x1*x1+x2*x2; ax2x1=atan2(x2,x1);
        //printf("kernel x1x2 = %f\n",x1x2);
        /* Calcul du champ et sommation ds composantes */ 
        u2.x =u2.x+be[kk]*(ax2x1+x1*x2/x1x2*uaux1);
        u2.y =u2.y-be[kk]*(uaux2*log(x1x2)+(x1*x1-x2*x2)/x1x2)*uaux;
        u2.z =u2.z+bs[kk]*ax2x1;
      }
    }
    u1[k]=u2;
    r1[k]=r11;
  }
}

/*
Pour chaque point aleatoire 1 <= p <= Np
On calcule la contribution au champ
*/
__kernel void comptf(__global double16 *Vect16FC, 
                     __global double8 *Vect8FC, 
                     __global double *Vect1FC,
                     __global double2 *rd0, 
                     __global double2 *r1, 
                     __global double3 *u1, 
                     __global double *be, 
                     __global double *bs, 
                       double Radius,
                       double nu,
                       int Np,
                       int   IndexFourier,
                       double  gs,
                       double3 gd,
                       double2 a3vd,
                       double a3,
                       int Nd,
                     __global int *inout)
{
  /*-- compute partial contribution of the Fourier coeffcients --*/	 
  // Np threads are launched 
  double uaux =1.0f/(4.0f*(1.0f-nu));
  double uaux1=1.0f/(2.0f*(1-nu));
  double uaux2=1.0f-2.0f*nu;
    
  /* Initialize local variables related to the Fourier Coefficients */
  double cAL  = 0.0f;
  double sAL  = 0.0f;
  double cAL2 = 0.0f;
  double sAL2 = 0.0f;
  double c2AL = 0.0f;
  double s2AL = 0.0f;
  double c2AL2= 0.0f;
  double s2AL2= 0.0f;
  double eps2l= 0.0f;
  
  /* Add other approximation for Harmonic rank : 3 */
  double cAL3 = 0.0f;
  double sAL3 = 0.0f;
  double c3AL = 0.0f;
  double s3AL = 0.0f;
  double c3AL2= 0.0f;
  double s3AL2= 0.0f;

/* Add other approximation for Harmonic rank : 4 */
  double cAL4 = 0.0f;
  double sAL4 = 0.0f;
  double c4AL = 0.0f;
  double s4AL = 0.0f;
  double c4AL2= 0.0f;
  double s4AL2= 0.0f;

/* Add other approximation for Harmonic rank : 5 */
  double cAL5 = 0.0f;
  double sAL5 = 0.0f;
  double c5AL = 0.0f;
  double s5AL = 0.0f;
  double c5AL2= 0.0f;
  double s5AL2= 0.0f;

  /* -- variables locales au calcul des deplacements */
  double3 u2=(double3)(0.0f,0.0f,0.0f);
  double2 r2=(double2)(0.0f,0.0f);
  double2 r0=(double2)(0.0f,0.0f);
  double  x1   =0.0f;
  double  x2   =0.0f;
  double  x1x2 =0.0f;
  double  ax2x1=0.0f;
  
  double c21=0.0f;
  double s21=0.0f;
  double sz_gu=0.0f;
  double3 du=(double3)(0.0f);
  double2 bb=(double2)(0.0f);  
  int flaginside=0;
  int inside_sin=0;
  int inside    =0;

  double s1_21=0.0f;
  double s2_21=0.0f;
  double s3_21=0.0f;
  double s4_21=0.0f;
  double s5_21=0.0f;


  /*---*/
  // Get k index of the random point i.e. 0 <= k < Np 	
  size_t k = get_global_id(0);
  
  /*=== Initialize the variables used in the computation
       of the Fourier Transform ===*/ 
  inout[k]=0;
  Vect16FC[k]=(double16)(0.0f);
  Vect8FC[k]=(double8)(0.0f);
  Vect1FC[k]=0.0f;
  /*=== ===*/

  if ( (int)k < Np )
  { 
    /*==== step 1 : compute the displacement field ===*/ 
    /* compute displacement cell */	
    bb=(double)(IndexFourier)*a3vd; 
    r2=r1[k]+bb; /* vector operation */
    if (length(r2) > Radius )
    {
      r2=r1[k]-bb; 
      if (length(r2) < Radius )
      {
        /* --- We are inside the sphere --- */
        inout[k]=1; // True 
        inside_sin=1;

        /*---  Compute the displacement field generated by all the  dislocations on point r2 ----*/
        /*     the function code has to be duplicated 
               since pointers are used in the argument */
        u2=(double3)(0.0f,0.0f,0.0f);
        /* Loop over all dislocation points */
        for (int kk=0;kk < Nd; kk++) 
        {
          /* r0= distance entre r2 et la kk-ime dislocation */
          r0=r2-rd0[kk];
          /* variables scalaires */
          x1  =r0.x; x2  =r0.y; x1x2=x1*x1+x2*x2; ax2x1=atan2(x2,x1);
          /* Calcul du champ genere par la kk-ieme dislocation et sommation ds composantes */ 
          u2.x =u2.x+be[kk]*(ax2x1+x1*x2/x1x2*uaux1);
          u2.y =u2.y-be[kk]*(uaux2*log(x1x2)+(x1*x1-x2*x2)/x1x2)*uaux;
          u2.z =u2.z+bs[kk]*ax2x1;
        } /* for (int kk=0;kk < Nd; kk++) */
        /* compute increment */
        du=u2-u1[k];
        /*-- Compute datas related to the Fourier Coefficients */
        sz_gu = dot(gd,du);
        /*-- Cumul quantities --*/
        eps2l  =(sz_gu/gs/(a3*(double)(IndexFourier)));
        eps2l  *=eps2l;
        c21    =cos(sz_gu);      cAL =c21;  cAL2=c21*c21;
        c21    =cos(2.0f*sz_gu); c2AL=c21; c2AL2=c21*c21;
        c21    =cos(3.0f*sz_gu); c3AL=c21; c3AL2=c21*c21;
        c21    =cos(4.0f*sz_gu); c4AL=c21; c4AL2=c21*c21;
        c21    =cos(5.0f*sz_gu); c5AL=c21; c5AL2=c21*c21;

	s1_21  =((inside_sin==1) ? -1:1 )*sin(     sz_gu); sAL =s1_21; sAL2=s1_21*s1_21;
	s2_21  =((inside_sin==1) ? -1:1 )*sin(2.0f*sz_gu); s2AL=s2_21; s2AL2=s2_21*s2_21;
	s3_21  =((inside_sin==1) ? -1:1 )*sin(3.0f*sz_gu); s3AL=s3_21; s3AL2=s3_21*s3_21;
	s4_21  =((inside_sin==1) ? -1:1 )*sin(4.0f*sz_gu); s4AL=s4_21; s4AL2=s4_21*s4_21;
	s5_21  =((inside_sin==1) ? -1:1 )*sin(5.0f*sz_gu); s5AL=s5_21; s5AL2=s5_21*s5_21;

        /*-- store the 8+1 coefficients used for computing the Fourier coefficients --*/
        Vect8FC[k]=(double8)(cAL,sAL,c2AL,s2AL,cAL2,sAL2,c2AL2,s2AL2);
        /* coeff for rank 3, 4, 5*/
        Vect16FC[k]=(double16)(c3AL,s3AL,c3AL2,s3AL2,c4AL,s4AL,c4AL2,s4AL2,c5AL,s5AL,c5AL2,s5AL2,0.0,0.0,0.0,0.0);
        Vect1FC[k]=eps2l;
      }
    }
    else  /**  here inside=true **/
    {
      /*--- point is inside ---*/
      inout[k]=1;
      /*---  Compute the displacement field generated by all the  dislocations on point r2 ----*/
      /*     the function code has to be duplicated 
             since pointers are used in the argument */
      u2=(double3)(0.0f,0.0f,0.0f);
      /* Loop over all dislocation points */
      for (int kk=0;kk < Nd; kk++) 
      {
        /* r0= distance entre r2 et la kk-ime dislocation */
        r0=r2-rd0[kk];
        /* variables scalaires */
        x1  =r0.x; x2  =r0.y; x1x2=x1*x1+x2*x2; ax2x1=atan2(x2,x1);
        /* Calcul du champ genere par la kk-ieme dislocation et sommation ds composantes */ 
        u2.x =u2.x+be[kk]*(ax2x1+x1*x2/x1x2*uaux1);
        u2.y =u2.y-be[kk]*(uaux2*log(x1x2)+(x1*x1-x2*x2)/x1x2)*uaux;
        u2.z =u2.z+bs[kk]*ax2x1;
      } /* for (int kk=0;kk < Nd; kk++) */
      /* compute increment */
      du=u1[k]-u2;
      /*-- Compute datas related to the Fourier Coefficients */
      sz_gu = dot(gd,du);
      /*-- Cumul quantities --*/
      eps2l  =(sz_gu/gs/(a3*(double)(IndexFourier)));
      eps2l  *=eps2l;
      c21    =cos(sz_gu);      cAL =c21;  cAL2=c21*c21;
      s21    = sin(sz_gu);     sAL =s21;  sAL2=s21*s21;
      c21    =cos(2.0f*sz_gu); c2AL=c21; c2AL2=c21*c21;
      s21    =sin(2.0f*sz_gu); s2AL=s21; s2AL2=s21*s21;

      c21    =cos(3.0f*sz_gu); c3AL=c21; c3AL2=c21*c21;
      c21    =cos(4.0f*sz_gu); c4AL=c21; c4AL2=c21*c21;
      c21    =cos(5.0f*sz_gu); c5AL=c21; c5AL2=c21*c21;

      s1_21  =((inside_sin==1) ? -1:1 )*sin(     sz_gu); sAL =s1_21; sAL2=s1_21*s1_21;
      s2_21  =((inside_sin==1) ? -1:1 )*sin(2.0f*sz_gu); s2AL=s2_21; s2AL2=s2_21*s2_21;
      s3_21  =((inside_sin==1) ? -1:1 )*sin(3.0f*sz_gu); s3AL=s3_21; s3AL2=s3_21*s3_21;
      s4_21  =((inside_sin==1) ? -1:1 )*sin(4.0f*sz_gu); s4AL=s4_21; s4AL2=s4_21*s4_21;
      s5_21  =((inside_sin==1) ? -1:1 )*sin(5.0f*sz_gu); s5AL=s5_21; s5AL2=s5_21*s5_21;

      /*-- store the 8+1 coefficients used for computing the Fourier coefficients --*/
      Vect8FC[k]=(double8)(cAL,sAL,c2AL,s2AL,cAL2,sAL2,c2AL2,s2AL2);
      /* coeff for rank 3, 4, 5*/
      Vect16FC[k]=(double16)(c3AL,s3AL,c3AL2,s3AL2,c4AL,s4AL,c4AL2,s4AL2,c5AL,s5AL,c5AL2,s5AL2,0.0,0.0,0.0,0.0);
      Vect1FC[k]=eps2l;
    }
  /*=========== =================*/ 
  }  	 
}

