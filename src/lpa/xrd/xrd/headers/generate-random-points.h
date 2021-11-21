size_t irand = sizeof(cl_double) * Np;
printf("memory space for random points positions: %lu Bytes\n", irand);

// initialize Mersenne Twister random number generator
sgenrand(4357);

if (FLAG_CYLINDER == 1) {

    random1 = (cl_double *)malloc(irand);
    for (i=0; i<Np; i++) {
      random1[i] = sqrt((double)(genrand()));
    }

    random2 = (cl_double *)malloc(irand);
    for (i=0; i<Np; i++) {
      random2[i] = (double)(genrand());
    }

} else {

  random1 = (cl_double *)malloc(irand);
  for (i=0; i<Np; i++) {
    random1[i] = (double)(genrand());
  }

  random2 = (cl_double *)malloc(irand);
  for (i=0; i<Np; i++) {
    random2[i] = (double)(genrand());
  }

}
