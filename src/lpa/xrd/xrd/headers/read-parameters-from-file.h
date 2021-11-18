FILE *fbe;
char buff[BUZZ_SIZE];
int idmod;
int i;

cl_double3 ld; // direction of 'l' (line vector) [uvw]
cl_double3 bd; // Burgers vector [nm]
cl_double3 ex; // direction of the Fourier variable [1]
cl_double3 ey; // cross product of ez and ex [1]
cl_double3 ez; // normalized line vector [1]
cl_double3 H; // diffraction vector direction (hkl)
cl_double3 eg; // normalized diffraction vector [1]
cl_double a_cell_param; // lattice parameter [nm]
cl_double cfact_str;
cl_double bsd; // Burgers vector screw part norm [nm]
cl_double3 bscrew; // Burgers vector screw part [nm]
cl_double3 bedge; // Burgers vector edge part [nm]
cl_double bed; // Burgers vector edge part norm [nm]
cl_double3 egd; // normalized Burgers vector [1]
cl_double3 gd;
cl_double3 a3v;
cl_double2 a3vd;
cl_double Dx;
cl_double Dy;
cl_int Np; // number of random points for the Monte Carlo method
cl_double a3; // Fourier variable step size [nm]
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
fscanf(fbe, "%lf %lf %lf", &ld.x, &ld.y, &ld.z);
printf("direction of 'l' (line vector): %1.0f %1.0f %1.0f\n", ld.x, ld.y, ld.z);

fgets(buff, BUZZ_SIZE, fbe);
fscanf(fbe, "%lf %lf %lf" ,&ex.x, &ex.y, &ex.z);
printf("direction of 'L' (Fourier variable): %1.0f %1.0f %1.0f\n", ex.x, ex.y, ex.z);

fgets(buff, BUZZ_SIZE, fbe);
fscanf(fbe,"%lf %lf %lf",&bd.x,&bd.y,&bd.z);
printf("Burgers vector direction: %1.0f %1.0f %1.0f\n", bd.x, bd.y, bd.z);

ez.x = ld.x;
ez.y = ld.y;
ez.z = ld.z;
double ezl = sqrt(ez.x*ez.x+ez.y*ez.y+ez.z*ez.z);
ez.x /= ezl;
ez.y /= ezl;
ez.z /= ezl;
printf("normalized line vector: %f %f %f\n", ez.x, ez.y, ez.z);

fgets(buff, BUZZ_SIZE, fbe);
fscanf(fbe,"%lf %lf %lf",&H.x,&H.y,&H.z);
printf("diffraction vector direction: %1.0f %1.0f %1.0f\n", H.x, H.y, H.z);

eg.x = H.x;
eg.y = H.y;
eg.z = H.z;
double egl = sqrt(eg.x*eg.x + eg.y*eg.y + eg.z*eg.z);
eg.x /= egl;
eg.y /= egl;
eg.z /= egl;
printf("normalized diffraction vector: %f %f %f\n", eg.x, eg.y, eg.z);

fgets(buff, BUZZ_SIZE, fbe);
fscanf(fbe, "%lf", &cfact_str);
printf("contrast coefficient: %lf\n", cfact_str);

fgets(buff, BUZZ_SIZE, fbe);
fscanf(fbe, "%lf", &a_cell_param);
printf("cell parameter: %lf nm\n", a_cell_param);

bd.x *= a_cell_param/2;
bd.y *= a_cell_param/2;
bd.z *= a_cell_param/2;
printf("Burgers vector: %f %f %f nm\n", bd.x, bd.y, bd.z);

bsd = ez.x*bd.x + ez.y*bd.y + ez.z*bd.z;
printf("Burgers vector screw part norm: %lf\n", bsd);

bscrew.x = bsd * ez.x;
bscrew.y = bsd * ez.y;
bscrew.z = bsd * ez.z;
printf("Burgers vector screw part: %f %f %f nm\n", bscrew.x, bscrew.y, bscrew.z);

bedge.x = bd.x - bscrew.x;
bedge.y = bd.y - bscrew.y;
bedge.z = bd.z - bscrew.z;
printf("Burgers vector edge part: %f %f %f nm\n", bscrew.x, bscrew.y, bscrew.z);

bed = length3(bedge);
printf("Burgers vector edge part norm: %lf\n", bed);

if (bsd < EPS) {
  printf("edge dislocations detected\n");
  bed = length3(bd);
  bsd = 0.0;
} else if (bed < EPS) {
  printf("screw dislocations detected\n");
  bsd = length3(bd);
  bed = 0.0;
} else {
  printf("mixed dislocations detected\n");
  ex.x = bedge.x;
  ex.y = bedge.y;
  ex.z = bedge.z;
}

cl_double exl = length3(ex);
ex.x /= exl;
ex.y /= exl;
ex.z /= exl;
printf("normalized Fourier variable direction: %f %f %f\n", ex.x, ex.y, ex.z);

ey.x = ez.y*ex.z - ez.z*ex.y;
ey.y = ez.z*ex.x - ez.x*ex.z;
ey.z = ez.x*ex.y - ez.y*ex.x;
printf("cross product of ez and ex: %f %f %f\n", ey.x, ey.y, ey.z);

// 1: create the 3x3 matrix

cl_double Om[3][3];

Om[0][0] = ex.x;
Om[0][1] = ey.x;
Om[0][2] = ez.x;

Om[1][0] = ex.y;
Om[1][1] = ey.y;
Om[1][2] = ez.y;

Om[2][0] = ex.z;
Om[2][1] = ey.z;
Om[2][2] = ez.z;

Om[0][0] = ex.x;
Om[0][1] = ex.y;
Om[0][2] = ex.z;

Om[1][0] = ey.x;
Om[1][1] = ey.y;
Om[1][2] = ey.z;

Om[2][0] = ez.x;
Om[2][1] = ez.y;
Om[2][2] = ez.z;

printf("|%9.6f %9.6f %9.6f|\n", Om[0][0], Om[0][1], Om[0][2]);
printf("|%9.6f %9.6f %9.6f|\n", Om[1][0], Om[1][1], Om[1][2]);
printf("|%9.6f %9.6f %9.6f|\n", Om[2][0], Om[2][1], Om[2][2]);

// 2:

cl_double coeff = length3(H)/a_cell_param;
cl_double3 g;

g.x = coeff*eg.x;
g.y = coeff*eg.y;
g.z = coeff*eg.z;

// 3: product matrix vector Om by g: gd = Om g

gd.x = Om[0][0]*g.x + Om[0][1]*g.y + Om[0][2]*g.z;
gd.y = Om[1][0]*g.x + Om[1][1]*g.y + Om[1][2]*g.z;
gd.z = Om[2][0]*g.x + Om[2][1]*g.y + Om[2][2]*g.z;

printf("gd.x = %lf\n", gd.x);
printf("gd.y = %lf\n", gd.y);
printf("gd.z = %lf\n", gd.z);

egd.x = gd.x;
egd.y = gd.y;
egd.z = gd.z;
cl_double norm=length3(egd);
egd.x /= norm;
egd.y /= norm;
egd.z /= norm;

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

a3v.x = a3 * egd.x;
a3v.y = a3 * egd.y;
a3v.z = a3 * egd.z;
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
