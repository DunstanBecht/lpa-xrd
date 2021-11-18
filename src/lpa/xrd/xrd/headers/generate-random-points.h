size_t irand = sizeof(cl_double) * Np;
printf("memory space for random points positions: %lu Bytes\n", irand);

// initialize Mersenne Twister random number generator
sgenrand(4357);

if (FLAG_CYLINDER == 1) {

    RandRadius = (cl_double *)malloc(irand);
    for (i=0; i<Np; i++) {
      RandRadius[i] = sqrt((double)(genrand()));
    }

    RandAngle = (cl_double *)malloc(irand);
    for (i=0; i<Np; i++) {
      RandAngle[i] = (double)(genrand());
    }

} else {

  RandX = (cl_double *)malloc(irand);
  for (i=0; i<Np; i++) {
    RandX[i] = (double)(genrand());
  }

  RandY = (cl_double *)malloc(irand);
  for (i=0; i<Np; i++) {
    RandY[i] = (double)(genrand());
  }

}
