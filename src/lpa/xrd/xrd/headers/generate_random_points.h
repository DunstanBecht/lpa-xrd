size_t bytes_random = sizeof(cl_double) * Np;
printf("memory space for random points positions: %lu bytes\n", bytes_random);

/* --- initialize Mersenne Twister random number generator ----------------- */

sgenrand(4357);

if (FLAG_CYLINDER == 1) {

    random1 = (cl_double *)malloc(bytes_random);
    for (i=0; i<Np; i++) {
      random1[i] = sqrt((double)(genrand()));
    }

    random2 = (cl_double *)malloc(bytes_random);
    for (i=0; i<Np; i++) {
      random2[i] = (double)(genrand());
    }

} else {

  random1 = (cl_double *)malloc(bytes_random);
  for (i=0; i<Np; i++) {
    random1[i] = (double)(genrand());
  }

  random2 = (cl_double *)malloc(bytes_random);
  for (i=0; i<Np; i++) {
    random2[i] = (double)(genrand());
  }

}
