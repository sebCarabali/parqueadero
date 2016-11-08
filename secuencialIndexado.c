#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXTAM 200

typedef struct
{
    char codigo[7],
    char nombre[32],
    char apellido[2][32];
} Articulo;

typedef struct
{
    char nombre[32];
    long posicion;
} Indice;


