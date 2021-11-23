output = fopen(argv[6], "w");
printf("output file: %s\n", argv[6]);

/* --- write header data --------------------------------------------------- */

fprintf(output, "%8s # v: lpa-xrd version\n", "!VERSION");
fprintf(output, "%8.2e # d: dislocation density [m^-2]\n", density);
fprintf(output, "%2.0f %2.0f %2.0f # z: direction of 'l' (line vector) [uvw]\n", l_uvw.x, l_uvw.y, l_uvw.z);
fprintf(output, "%2.0f %2.0f %2.0f # b: Burgers vector direction [uvw]\n", b_uvw.x, b_uvw.y, b_uvw.z);
fprintf(output, "%2.0f %2.0f %2.0f # g: diffraction vector direction (hkl)\n", g_hkl.x, g_hkl.y, g_hkl.z);
fprintf(output, "%8f # C: contrast coefficient [1]\n", cfact_str);
fprintf(output, "%8f # a: cell parameter [nm]\n", a_cell_param);
if (FLAG_SQUARE==1) {
  fprintf(output, "%8.0f # s: side of the region of interest [nm]", size);
  if (replications>0) {
    fprintf(output, " PBC%d", replications);
  }
  fprintf(output, "\n");
} else {
  fprintf(output, "%8.0f # s: radius of the region of interest [nm]\n", size);
}
fprintf(output, "%8f # nu: Poisson's number [1]\n", nu);
fprintf(output, "%8d # nd: number of dislocations in the input file\n", Nd0);
fprintf(output, "%8d # np: number of random points\n", Np);

/* --- write column names -------------------------------------------------- */

fprintf(output, "# %4s", "L");

for (i=1; i<=HARMONICS; i++){
  fprintf(output, " %9s%d %9s%d %9s%d %9s%d", "cos", i, "err_cos", i, "sin", i, "err_sin", i);
}
fprintf(output, " %10s %10s\n", "<eps^2>", "bad_points");

/* --- write table --------------------------------------------------------- */

for (i=0; i<Nf; i++) {
  fprintf(output, "%6.1lf", (i+1)*a3);
  for (j=0; j<HARMONICS; j++){
    fprintf(output,
      " %10.7f %10.7f %10.7f %10.7f",
      res_cos[i][j],
      res_cos_std[i][j],
      res_sin[i][j],
      res_sin_std[i][j]);
  }
  fprintf(output, " %10.7f %10d\n", res_eps[i], Np-res_nrp[i]);
}

/* --- dump results to files for debugging --------------------------------- */

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

/* --- release memory ------------------------------------------------------ */

printf("free memory references from device\n");

ret = clFlush(queue);
ret |= clFinish(queue);
ret |= clReleaseKernel(kernel1);
ret |= clReleaseKernel(kernel2);
ret |= clReleaseProgram(program);
ret |= clReleaseCommandQueue(queue);
ret |= clReleaseContext(context);

if (ret != CL_SUCCESS) {
  printf("error encountered while decrementing references counts\n");
  exit(EXIT_FAILURE);
}

ret = clReleaseMemObject(d_dislocations);
ret |= clReleaseMemObject(d_random1);
ret |= clReleaseMemObject(d_random2);
ret |= clReleaseMemObject(d_inout);
ret |= clReleaseMemObject(d_r1);
ret |= clReleaseMemObject(d_u1);
ret |= clReleaseMemObject(d_Vect16FC);

if (ret != CL_SUCCESS) {
  printf("error encountered while decrementing the memory object references counts\n");
  exit(EXIT_FAILURE);
}

printf("free memory references from host\n");

free(senses);
free(positions);
free(dislocations);
free(random1);
free(random2);
free(h_r1);
free(h_u1);
free(h_inout);
free(h_Vect16FC);
