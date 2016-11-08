#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

FILE *fb = NULL;
const char fich[] = "fichero.dat";
typedef struct
{
    int num;
    char nif [13];
    char nombre[45];
} Habitacion;

#define numhab 55
#define desplazamiento(n) ((n-1) * sizeof(Habitacion))

void inauguracion(void);
void entrada(void);
void salida(void);
void ocupadas(void);
void leerRes(Habitacion *ph);
void escribirRes(Habitacion h);

void main()
{
    char opcion;
    do {
        puts("1. Inauguración");
        puts("2. Entrada");
        puts("3. Salida");
        puts("4. Ocupadas");
        puts("5. Salir");
        
        do {
            printf("Elige una opción ");
            scanf("%c%*c", &opcion);
        } while(opcion < '1' || opcion > '5');
        
        switch(opcion)
        {
            case '1':
                inauguracion();
                break;
            case '2':
                entrada();
                break;
            case '3':
                salida();
                break;
            case '4':
                ocupadas();
                system("pause");
                break;
                
        }
    } while(opcion != '5');
    if(fb != NULL) fclose(fb);
}

void inauguracion(void)
{
    Habitacion h;
    int i;
    if(fb != NULL)
    {
        char opcion;
        printf ("Archivo ya creado, ¿ desea continuar(S/N) ?: ");
        scanf ("%c%*c", &opcion);
        if (toupper(opcion) != 'S') return;
    }
    fb = fopen(fich, "wb+");
    for(i = 1; i <= numhab; i++)
    {
        h.num = i;
        strcpy(h.nif, "*");
        fwrite(&h, sizeof(h), 1, fb);
    }
    fflush(fb);
}

void entrada(void)
{
    Habitacion h;
    int encontrado, nh;
    encontrado = nh = 0;
    if( fb == NULL) fb = fopen(fich, "rb+");
    fseek(fb, 0L, SEEK_SET);
    while( (nh < numhab) && !encontrado)
    {
        fread(&h, sizeof(h), 1, fb);
        nh++;
        if(strcmp(h.nif, "*") == 0)
        {
            encontrado = 1;
            leerRes(&h);
            fseek(fb, desplazamiento (h.num), SEEK_SET);
            fwrite(&h, sizeof(h), 1, fb);
            puts("Datos grabados");
        }
    }
    if(!encontrado) puts ("Hotel completo");
    fflush(fb);
}

void salida(void)
{
    Habitacion h;
    int n;
    char r;
    if( fb == NULL) fb = fopen(fich, "rb+");
    printf("Numero de habitacion: ");
    scanf("%d%*c", &n);
    fseek(fb, desplazamiento (n), SEEK_SET);
    fread(&h, sizeof (h), 1, fb);
    if(strcmp(h.nif, "*") != 0)
    {
        escribirRes (h);
        printf ("¿Son correctos los datos?(S/N) ");
        scanf ("%c%*c" ,&r);
        if (toupper (r) == 'S')
        {
            strcpy (h.nif, "*");
            fseek (fb, -sizeof (h), SEEK_CUR);/* se posiciona de nuevo */
            fwrite (&h, sizeof (h), 1, fb);
        }
    }
    else
        puts ("La habitacion figura como libre");
    fflush (fb);
}

void ocupadas (void) {
    Habitacion h;
    int n;
    if (fb == NULL) fb = fopen (fich, "rb+");
    fseek (fb, 0L, SEEK_SET);
    puts (" Numero \t NIF \t\t Nombre");
    puts (" habitacion ");
    for (n = 1 ; n <= numhab; n++)
    {
        fread (&h, sizeof (h), 1, fb);
        if (strcmp (h.nif ,"*") != 0)
            escribirRes (h);
    }
}
void leerRes (Habitacion *ph)
{
    printf ("Nif: ");
    fgets (ph->nif, 13, stdin);
    printf ("Nombre ");
    fgets (ph->nombre, 45, stdin);
}
void escribirRes (Habitacion h)
{
    printf ("\t %d", h.num);
    printf ("\t%s\t", h.nif);
    printf ("\t%s\n", h.nombre);
}
