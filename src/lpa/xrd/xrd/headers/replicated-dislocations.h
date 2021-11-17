  /* allocate variable for the dislocations including */
  printf("...may 2021 define replicated\n");
  /* -- allocate vector -- */  
  rd0_all = (cl_double3 *)malloc(sizeof(cl_double3)*Nd);
  
  size_t isd0_all=sizeof(cl_int)*Nd;
  printf("isd0_all= %10lu  Bytes\n",isd0_all);
  printf("isd0_all= %10lu kBytes\n",isd0_all/1024);

  int index_all=-1;
  int ii,jj;
  int idislo=0;

  printf("... create replicated cells of the central cell by translation along x and y\n");
  /* this loop works also for the cylindrical case */
  /* along X and Y directions it computes the position of the dislocation */
  /* The z component contains the sign of the dislocation direction       */
  for (ii=-D_REPLICATION; ii<= D_REPLICATION; ii++)
  {
    for (jj=-D_REPLICATION; jj<= D_REPLICATION; jj++)
    {
      for (idislo=0; idislo< Nd0 ; idislo++)
      {
        index_all=index_all+1;
        rd0_all[index_all].x=rd0[idislo].x+ii*Radius;
        rd0_all[index_all].y=rd0[idislo].y+jj*Radius;
        rd0_all[index_all].z=(double)(sd0[idislo]);
      } 
    }
  }
  int NbDisloWithReplication=index_all;
  printf("....Nb Dislocations with Replications = %d\n",NbDisloWithReplication);

  /*  dump data to file */
  FILE *FileAllDislo;
  FileAllDislo=fopen("AllDislo.txt","w");
  if (FileAllDislo==NULL)
  {
    printf("Error in creating AllDislo.txt file, exit \n");
    exit(1);
  }
  for (ii=0; ii <= NbDisloWithReplication; ii++)
    fprintf(FileAllDislo,"%e %e %e\n",rd0_all[ii].x,rd0_all[ii].y,rd0_all[ii].z);
  fclose(FileAllDislo);

  printf("BCtype = %d\n",BCtype);
  
  /*-- constants --*/
  uaux =1-nu;
  uaux1=2*uaux;
  uaux2=1-2.0*nu;
  printf("nu     = %lf\n",nu);
  printf("uaux   = %lf\n",uaux);
  printf("uaux1  = %lf\n",uaux1);
  printf("uaux2  = %lf\n\n",uaux2);
