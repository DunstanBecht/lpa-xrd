// dump results to file for analysis
if (DUMP == 1){

  FILE *fr;
  fr = fopen("r.res", "w");
  for (i=0; i<Np; i++) {
    fprintf(fr, "%lf %lf\n", h_r1[i].x, h_r1[i].y);
  }
  fclose(fr);
  printf("dump 2d position field\n");

  FILE *fu;
  fu = fopen("u.res", "w");
  for (i=0; i<Np; i++) {
    fprintf(fu, "%lf %lf %lf\n", h_u1[i].x, h_u1[i].y, h_u1[i].z);
  }
  fclose(fu);
  printf("dump 3d displacement field\n");

}
