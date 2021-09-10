#define  _GNU_SOURCE
#include <stdio.h>
#define  _POSIX_C_SOURCE 200809L
#include <stdio.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include<limits.h>
#include <string.h>
#include <ctype.h>

typedef struct Parametre Parametre;
struct Parametre {
  int nbRows;
  int nbColumns;
  double ecartMin;
  double ecartMax;
  double alpha;
};
typedef struct Liste Liste;
struct Liste
{
    int nombre;
    Liste *suivant;
};

typedef struct Data Data;
struct Data {
  double* Data;
  char * nom;
  int taille;
  int DataSize;
  int type;
};

typedef struct Neurone Neurone;
struct Neurone {
  double * vect;
  int taille;
  int type;
  int setoza;
  int verginica;
  int vergicono;
  int nbClusters;
};
