FILE *fbe;
char buff[BUZZ_SIZE];
int idmod;
int i;

cl_double3 l_uvw; // direction of 'l' (line vector) [uvw]
cl_double3 l_uni; // normalized direction of 'l' (line vector) [1]
cl_double3 b_uvw; // Burgers vector direction [uvw]
cl_double3 b_vec; // Burgers vector [nm]
cl_double3 L_uvw; // direction of 'L' (Fourier variable) [uvw]
cl_double3 L_uni; // normalized direction of 'L' (Fourier variable) [1]
cl_double3 g_hkl; // diffraction vector direction (hkl)
cl_double3 g_vec; // diffraction vector [nm^-1]
cl_double3 g_uni; // normalized diffraction vector [1]
cl_double3 d_uni; // cross product of l_uni and L_uni [1]
cl_double a_cell_param; // lattice parameter [nm]
cl_double bs_len; // Burgers vector screw part norm [nm]
cl_double3 bs_vec; // Burgers vector screw part [nm]
cl_double3 be_vec; // Burgers vector edge part [nm]
cl_double be_len; // Burgers vector edge part norm [nm]
cl_double3 gd_vec; // diffraction vector in slip frame [nm^-1]
cl_double3 gd_uni; // normalized diffraction vector in slip frame [1]

cl_double cfact_str;

cl_double a3; // Fourier variable step size [nm]
cl_double3 a3v;
cl_double2 a3vd;

cl_double Dx;
cl_double Dy;
cl_int Np; // number of random points for the Monte Carlo method

cl_int NoFC; // number of values taken by the Fourier variable
cl_int BCtype;
cl_double nu; // Poisson's number [1]
cl_int Nd0; // number of dislocations in the input file
cl_double3 *rd0; // (x,y) dislocations coordinates [nm]
cl_double3 *rd0_all; // (x,y) dislocations coordinates [nm] and Burgers vector senses [1]
cl_int *sd0; // Burgers vector senses [1]
cl_int *sd0_all;
cl_int Nd; // total number of dislocations
cl_double *be;
cl_double *bs;
cl_double *RandRadius; // random numbers for radius in polar coordinates
cl_double *RandAngle; // random numbers for angle in polar coordinates
cl_double *RandX; // random numbers for X in cartesian coordinates
cl_double *RandY; // random numbers for Y in cartesian coordinates
cl_double Radius;  // radius of the dislocation points
cl_double8 *FC;  // Pack 8 fourier coefficients
cl_int IndexFourier; // entier entre 0 et NoFC-1 pour le calcul de la distance

char version[8]; // lpa-input version
float density; // real density [m^-2]

Np = atoi(argv[4]);
printf("number of random points for the Monte Carlo method: %ld\n", Np);

NoFC = atoi(argv[5]);
printf("number of values taken by the Fourier variable: %ld\n", NoFC);

printf("input data file: %s\n", argv[3]);
fbe = fopen(argv[3], "r");

if (fbe == NULL) {
  printf("error while reading the input datafile\n");
  exit(1);
}

fscanf(fbe, "%s", &version);
printf("lpa-input version: %s\n", version);

fgets(buff, BUZZ_SIZE, fbe);
fscanf(fbe, "%e", &density);
printf("density: %1.2e m^-2\n", density);

fgets(buff, BUZZ_SIZE, fbe);
fscanf(fbe, "%lf %lf %lf", &l_uvw.x, &l_uvw.y, &l_uvw.z);
printf("direction of 'l' (line vector): %1.0f %1.0f %1.0f [uvw]\n", l_uvw.x, l_uvw.y, l_uvw.z);

fgets(buff, BUZZ_SIZE, fbe);
fscanf(fbe, "%lf %lf %lf", &L_uvw.x, &L_uvw.y, &L_uvw.z);
printf("direction of 'L' (Fourier variable): %1.0f %1.0f %1.0f [uvw]\n", L_uvw.x, L_uvw.y, L_uvw.z);

fgets(buff, BUZZ_SIZE, fbe);
fscanf(fbe,"%lf %lf %lf", &b_uvw.x, &b_uvw.y, &b_uvw.z);
printf("Burgers vector direction: %1.0f %1.0f %1.0f [uvw]\n", b_uvw.x, b_uvw.y, b_uvw.z);

double l_len = length3(l_uvw);
l_uni.x = l_uvw.x/l_len;
l_uni.y = l_uvw.y/l_len;
l_uni.z = l_uvw.z/l_len;
printf("normalized line vector: %f %f %f\n", l_uni.x, l_uni.y, l_uni.z);

fgets(buff, BUZZ_SIZE, fbe);
fscanf(fbe, "%lf %lf %lf", &g_hkl.x, &g_hkl.y, &g_hkl.z);
printf("diffraction vector direction: %1.0f %1.0f %1.0f (hkl)\n", g_hkl.x, g_hkl.y, g_hkl.z);

double g_len = length3(g_hkl);
g_uni.x = g_hkl.x/g_len;
g_uni.y = g_hkl.y/g_len;
g_uni.z = g_hkl.z/g_len;
printf("normalized diffraction vector: %f %f %f\n", g_uni.x, g_uni.y, g_uni.z);

fgets(buff, BUZZ_SIZE, fbe);
fscanf(fbe, "%lf", &cfact_str);
printf("contrast coefficient: %lf\n", cfact_str);

fgets(buff, BUZZ_SIZE, fbe);
fscanf(fbe, "%lf", &a_cell_param);
printf("cell parameter: %lf nm\n", a_cell_param);

b_vec.x = b_uvw.x * a_cell_param/2;
b_vec.y = b_uvw.y * a_cell_param/2;
b_vec.z = b_uvw.z * a_cell_param/2;
printf("Burgers vector: %f %f %f nm\n", b_vec.x, b_vec.y, b_vec.z);

bs_len = l_uni.x*b_vec.x + l_uni.y*b_vec.y + l_uni.z*b_vec.z;
printf("Burgers vector screw part norm: %lf\n", bs_len);

bs_vec.x = bs_len * l_uni.x;
bs_vec.y = bs_len * l_uni.y;
bs_vec.z = bs_len * l_uni.z;
printf("Burgers vector screw part: %f %f %f nm\n", bs_vec.x, bs_vec.y, bs_vec.z);

be_vec.x = b_vec.x - bs_vec.x;
be_vec.y = b_vec.y - bs_vec.y;
be_vec.z = b_vec.z - bs_vec.z;
printf("Burgers vector edge part: %f %f %f nm\n", bs_vec.x, bs_vec.y, bs_vec.z);

be_len = length3(be_vec);
printf("Burgers vector edge part norm: %lf\n", be_len);

if (bs_len < EPS) {
  printf("edge dislocations detected\n");
  be_len = length3(b_vec);
  bs_len = 0.0;
} else if (be_len < EPS) {
  printf("screw dislocations detected\n");
  bs_len = length3(b_vec);
  be_len = 0.0;
} else {
  printf("mixed dislocations detected\n");
  L_uni.x = be_vec.x; // why ?
  L_uni.y = be_vec.y;
  L_uni.z = be_vec.z;
}

cl_double L_len = length3(L_uvw);
L_uni.x = L_uvw.x/L_len;
L_uni.y = L_uvw.y/L_len;
L_uni.z = L_uvw.z/L_len;
printf("normalized Fourier variable direction: %f %f %f\n", L_uni.x, L_uni.y, L_uni.z);

d_uni.x = l_uni.y*L_uni.z - l_uni.z*L_uni.y;
d_uni.y = l_uni.z*L_uni.x - l_uni.x*L_uni.z;
d_uni.z = l_uni.x*L_uni.y - l_uni.y*L_uni.x;
printf("cross product of l_uni and L_uni: %f %f %f\n", d_uni.x, d_uni.y, d_uni.z);

// 1: create the 3x3 matrix

cl_double Om[3][3];

Om[0][0] = L_uni.x;
Om[0][1] = d_uni.x;
Om[0][2] = l_uni.x;

Om[1][0] = L_uni.y;
Om[1][1] = d_uni.y;
Om[1][2] = l_uni.y;

Om[2][0] = L_uni.z;
Om[2][1] = d_uni.z;
Om[2][2] = l_uni.z;

Om[0][0] = L_uni.x;
Om[0][1] = L_uni.y;
Om[0][2] = L_uni.z;

Om[1][0] = d_uni.x;
Om[1][1] = d_uni.y;
Om[1][2] = d_uni.z;

Om[2][0] = l_uni.x;
Om[2][1] = l_uni.y;
Om[2][2] = l_uni.z;

printf("|%9.6f %9.6f %9.6f|\n", Om[0][0], Om[0][1], Om[0][2]);
printf("|%9.6f %9.6f %9.6f|\n", Om[1][0], Om[1][1], Om[1][2]);
printf("|%9.6f %9.6f %9.6f|\n", Om[2][0], Om[2][1], Om[2][2]);

// 2:

g_vec.x = g_hkl.x/a_cell_param;
g_vec.y = g_hkl.y/a_cell_param;
g_vec.z = g_hkl.z/a_cell_param;

// 3: product matrix vector Om by g: gd = Om g

gd_vec.x = Om[0][0]*g_vec.x + Om[0][1]*g_vec.y + Om[0][2]*g_vec.z;
gd_vec.y = Om[1][0]*g_vec.x + Om[1][1]*g_vec.y + Om[1][2]*g_vec.z;
gd_vec.z = Om[2][0]*g_vec.x + Om[2][1]*g_vec.y + Om[2][2]*g_vec.z;

printf("gd_vec.x = %lf\n", gd_vec.x);
printf("gd_vec.y = %lf\n", gd_vec.y);
printf("gd_vec.z = %lf\n", gd_vec.z);

cl_double gd_len = length3(gd_vec);
gd_uni.x = gd_vec.x/gd_len;
gd_uni.y = gd_vec.y/gd_len;
gd_uni.z = gd_vec.z/gd_len;

// geometry:

fgets(buff, BUZZ_SIZE, fbe);
fscanf(fbe, "%lf", &Radius);
printf("characteristic size of the region of interest: %lf nm\n", Radius);

fgets(buff, BUZZ_SIZE, fbe);

char *resucyl = strstr(buff, "radius");
if (resucyl != NULL) {
  printf("circular region of interest detected\n");
  FLAG_CYLINDER = 1;
  FLAG_SQUARE = 0;
}

char *resusquare = strstr(buff, "side");
if (resusquare != NULL) {
  printf("square region of interest detected\n");
  FLAG_CYLINDER = 0;
  FLAG_SQUARE = 1;
  // get the value of the number of replications
  char numbers[10];
  strcat(numbers, "");
  //printf("line length: %ld\n", strlen(buff));
  int index = 0;
  for (i=0; i<strlen(buff); i++) {
    //printf("i=%d\n", i);
    if (isdigit(buff[i])) {
     //printf("ok");
     numbers[index] = buff[i];
     index += 1;
    }
  }
  //printf("string: %s\n", numbers);
  int rep = atoi(numbers);
  printf("number of replications: %d\n", rep);
  D_REPLICATION = rep;
}

fscanf(fbe, "%lf", &a3);
printf("Fourier variable step size: %lf nm\n", a3);

a3v.x = a3 * gd_uni.x;
a3v.y = a3 * gd_uni.y;
a3v.z = a3 * gd_uni.z;
printf("Fourier variable step vector: %lf %lf %lf nm\n", a3v.x, a3v.y, a3v.z);

a3vd.x = a3v.x;
a3vd.y = a3v.y;
printf("Fourier variable step vector projection: %lf %lf nm\n", a3vd.x, a3vd.y);

fgets(buff, BUZZ_SIZE, fbe);
fscanf(fbe, "%lf", &nu);
printf("Poisson's number: %lf\n", nu);

fgets(buff, BUZZ_SIZE, fbe);
fscanf(fbe, "%d", &Nd0);
printf("number of dislocations in the input file: %d\n", Nd0);

size_t ird0 = sizeof(cl_double3) * Nd0;
printf("memory space for positions: %lu Bytes\n", ird0);
rd0 = (cl_double3 *)malloc(sizeof(cl_double3)*Nd0);

size_t isd0 = sizeof(cl_int) * Nd0;
printf("memory space for Burgers vectors: %lu Bytes\n", isd0);
sd0 = (cl_int *)malloc(sizeof(cl_int)*Nd0);

fgets(buff, BUZZ_SIZE, fbe);
fgets(buff, BUZZ_SIZE, fbe);

for(i=0; i<Nd0; i++) {
  fscanf(fbe, "%d %le %le\n", &sd0[i], &rd0[i].x, &rd0[i].y);
  //printf("%2d %9.6e %9.6e \n", sd0[i], rd0[i].x, rd0[i].y);
}

fclose(fbe);

if (FLAG_SQUARE == 1) {
  Nd = Nd0 * (2*D_REPLICATION+1) * (2*D_REPLICATION+1);
} else {
  Nd = Nd0;
}

printf("total number of dislocations: %d\n", Nd);

rd0_all = (cl_double3 *)malloc(sizeof(cl_double3)*Nd);
size_t ird0_all = sizeof(cl_int) * Nd;
printf("memory space for positions and Burgers vectors: %lu Bytes\n", ird0_all);
