/** 28 juin 2021
---modification du calcul des des harmoniques des coefficients de fourier ordre 5
**/

#include "headers/square-cyl.h"
/*
------------------------
ENTRY POINT OF THE PROGRAM
------------------------
*/
int main(int argc, char **argv)
{
  printf("Version square-cyl-ocl-mt-double.c : june 2021\n");
  printf("3rd 4th 5th Fourier coefficients added in the opencl kernel\n");
  printf("...modification of .c code and .cl kernel code\n");
  printf("...handles both square and cylinder geometries for the placement of the dislocations *\n");
  printf("Calcul en double precision\n");
  printf("Affichage des resultats en notation scientifique\n");
  printf("prise en compte du mersenne twister pour random number generateur\n");
  printf("argc= %d\n",argc);
  if (argc != 7)
  {
    printf("Erreur arguments\n");
    printf("Mode d'emploi : ./a.out <type> <block> <ParameterFile> <Np> <NFC> <FC FileName>\n");
    printf("arg 1 : type= 0 : cpu\n");
    printf("arg 1 : type= 1 : gpu\n");
    printf("arg 2 : block : 512 --> from 2 to 256 depending on the gpu characteristics\n");
    printf("arg 3 : FileName of DataFile\n");
    printf("arg 4 : Np: Number of Random Points\n");
    printf("arg 5 : NoFC: Number of Fourier Coefficients\n");
    printf("arg 6 : File Name for the Fourier Coefficients\n");
    exit(1);
  }

  printf("\nSTEP 01\n");
  #include "headers/read-parameters-from-file.h"
  printf("\nSTEP 02\n");
  #include "headers/replicated-dislocations.h"
  printf("\nSTEP 03\n");
  #include "headers/generate-random-points.h"
  printf("\nSTEP 04\n");
  #include "headers/define-opencl-accelerator.h"
  printf("\nSTEP 05\n");
  #include "headers/create-kernel1.h"
  printf("\nSTEP 06\n");
  #include "headers/create-kernel2.h"
  printf("\nSTEP 07\n");
  #include "headers/results-kernel1.h"
  printf("\nSTEP 08\n");
  #include "headers/compute-fourier-coefficients-kernel2.h"
  printf("\nSTEP 09\n");
  #include "headers/compute-error-dump-results.h"
  printf("\nSTEP 10\n");
  #include "headers/release-ressources.h"
  printf("\nSTEP 11\n");
  #include "headers/compute-performance.h"
