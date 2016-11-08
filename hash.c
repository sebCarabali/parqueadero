#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define principal 199 // 0 - 198 Área principal del archivo
#define total 240 // 199 - 239 Área de desbordamiento o colisiones.
#define desplazamiento(n) ((n) * sizeof(Libro))

const char fich[12] = "fichash.dat";

FILE *fh = NULL;

typedef struct
{
    char codigo[7];
    char autor[41];
    char titulo[41];
} Libro;

void creacion(void);
void compra(void);
void venta(void);
void consulta(void);
void colisiones(Libro lib);
int indexSinonimo(const char c[]);
int hash(char c[]);
long transformaClave(const char c[]);
void escribir(Libro lib);

void main()
{
    char opcion;
    fh = fopen(fich, "rb");
    if (fh == NULL)
    {
        puts("El archivo va a ser creado");
        creacion();
    }
    else
        fh = NULL;
    do
    {
        puts("1. Compra ");
        puts("2. Venta ");
        puts("3. Consulta ");
        puts("0. Salir");
        do {
            printf("> Elige una opción: ");
            scanf("%c%*c", &opcion);
        } while( opcion < '0' || opcion > '3' );
        
        switch(opcion)
        {
            case '1':
                compra(); break;
            case '2':
                venta(); break;
            case '3':
                consulta(); break;
        }
    } while(opcion != '0');
    if (fh != NULL) fclose(fh);
}

void creacion(void)
{
    Libro lib;
    int i;
    fh = fopen(fich, "wb+");
    strcpy(lib.codigo, "*");
    for(i = 1; i <= total; i++)
        fwrite(&lib, sizeof(lib), 1, fh);
    fclose(fh);
    fh = NULL;
}

void compra(void)
{
    Libro lib, libar;
    long posicion;
    
    if(fh == NULL) fh = fopen(fich, "rb+");
    printf("Código: "); fgets(lib.codigo, 7, stdin);
    printf("Autor: "); fgets(lib.autor, 41, stdin);
    printf("Título: "); fgets(lib.titulo, 41, stdin);
    posicion = hash(lib.codigo);
    posicion = desplazamiento(posicion);
    
    fseek(fh, posicion, SEEK_SET);
    fread(&libar, sizeof(Libro), 1, fh);
    if(strcmp(libar.codigo, "*") == 0)
    {
        fseek(fh, -sizeof(Libro), SEEK_CUR);
        fwrite(&lib, sizeof(Libro), 1, fh);
        printf("Registro grabado en la dirección: %ld\n",posicion);
    } else if(strcmp(libar.codigo, lib.codigo) == 0)
    {
        puts("Código repetido, revisar los datos.");
        return;
    } else
        colisiones(lib);
    fflush(fh);
}

void venta()
{
    Libro libar;
    char codigo[7], r;
    long posicion;
    
    if (fh == NULL) fh = fopen(fich, "rb+");
    printf("Código: "); fgets(codigo, 7, stdin);
    posicion = hash(codigo);
    posicion = desplazamiento(posicion);
    
    fseek(fh, posicion, SEEK_SET);
    fread(&libar, sizeof(Libro), 1, fh);
    
    if(strcmp(libar.codigo, codigo) != 0)
        posicion = indexSinonimo(codigo);
    
    if(posicion != -1)
    {
        escribir(libar);
        printf("¿Son correctos los datos? (S/N): ");
        if (toupper(r) == 'S')
        {
            strcpy(libar. codigo, " *");
            fseek(fh, -sizeof(Libro), SEEK_CUR);
            fwrite(&libar, sizeof(Libro), 1, fh);
        }
    } else {
        puts("No se encuentra un registro con ese código.");
    }
    fflush(fh);
}

void consulta()
{
    Libro lib;
    char codigo[7];
    long posicion;
    
    if(fh == NULL) fh = fopen(fich, "rb+");
    
    printf("Código: "); fgets(codigo, 7, stdin);
    posicion = hash(codigo);
    posicion = desplazamiento(posicion);
    
    fseek(fh, posicion, SEEK_SET);
    fread(&lib, sizeof(Libro), 1, fh);
    
    if(strcmp(lib.codigo, codigo) == 0)
        escribir(lib);
    else
    {
        int posicion;
        posicion = indexSinonimo(codigo);
        if(posicion != -1)
        {
            fseek(fh, -sizeof(Libro), SEEK_CUR);
            fread(&lib, sizeof(Libro), 1, fh);
            escribir(lib);
        } else
            puts("No se encuentra un ejemplar con ese código.");
    }
}

void colisiones(Libro lib)
{
    Libro libar;
    int pos = desplazamiento(principal);
    int j = principal;
    int encontrado;
    fseek(fh, pos, SEEK_SET); /* se sitúa en área de sinónimos */
    encontrado = 0;
    while ((j < total) && !encontrado)
    {
        fread(&libar, sizeof(Libro), 1, fh);
        j++;
        if (strcmp(libar.codigo ,"*") == 0) /* libre */
        {
            encontrado = 1;
            fseek(fh, -sizeof(Libro), SEEK_CUR);
            fwrite(&lib, sizeof(Libro), 1, fh);
            puts("Datos grabados en el área de sinónimos.");
        }
    }
    if (!encontrado) puts("Área de sinónimos completa. ");
    fflush(fh);
}

int indexSinonimo(const char c[])
{
    Libro libar;
    int pos = desplazamiento(principal);
    int j = principal;
    int encontrado;
    fseek(fh, pos, SEEK_SET); /* se sitúa en área de sinónimos */
    encontrado = 0;
    
    while ((j < total) && !encontrado)
    {
        fread(&libar, sizeof(Libro), 1, fh);
        j++;
        if (strcmp(libar.codigo, c) == 0) {
            encontrado = 1;
        }
    }
    if(encontrado == 0)
        j = -1;
    //if(!encontrado) j = −1;
    return j;
}

int hash(char c [ ])
{
    int i, suma = 0;
    for (i = 0; i < strlen(c); i++) {
        c[i] = toupper(c[i]);
    }
    return transformaClave(c) % principal;
}

long transformaClave(const char* clave)
{
    int j;
    long d;
    d = 0;
    for (j = 0; j < strlen(clave); j++)
    {
        d = d * 27 + clave[j];
    }
    if(d < 0)
        d = -d;
    return d;
}

void escribir(Libro lib)
{
    printf("Código: %s\t", lib.codigo);
    printf("Autor: %s\t", lib.autor);
    printf("Título: %s\n" ,lib.titulo);
}
