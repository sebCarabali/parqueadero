#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "parqueadero.h"

/* pausa la ejecucion del programa -> system("pause") */
void
pausar_programa()
{
	printf("Presione la tecla 'ENTER' para continuar.\n");
	while (getchar() != '\n');
}

/* Elimina los caracteres de retorno de l?nea al final de cadena */
void
eliminar_retorno_linea(char *cad)
{
   int i;
   /* la funcion fgets captura el retorno de linea, hay que eliminarlo */
   for(i = strlen(cad)-1; i >= 0 && cad[i] < ' '; i--) cad[i] = 0;
}

/* calcula el tiempo en minutos que a estado el vehiculo ingresado en el parqueadero */
double
calcular_tiempo_en_minutos(Espacio_t espacio)
{
	time_t tiempo_inicial = espacio.hora_ingreso;
	time_t tiempo_final   = time(0);
	return difftime(tiempo_final, tiempo_inicial) / 60.0;
}

void
cargar_datos(Espacio_t *array_puestos, 
	unsigned int *cantidad_automoviles, unsigned int *cantidad_motocicletas)
{
	FILE *f = fopen(FILE_PATH ,"rb");
	if(!f) return;
	int i;
	Espacio_t espacio;
	i = 0;
	while(fread(&espacio, sizeof(Espacio_t), 1, f) == 1)
	{
		if(espacio.tipo_vehiculo == AUTOMOVIL) {
			*cantidad_automoviles = *cantidad_automoviles + 1;
		} else {
			*cantidad_motocicletas = *cantidad_motocicletas + 1;
		}
		array_puestos[i++] = espacio;
	}
	fclose(f);
}

int
menu_parqueadero()
{
  char opcion[2];
  do{
    system("clear");
    printf("***** Menu Parqueadero *****\n\n");
    printf("1. Ingreso de vehiculo.\n");
    printf("2. Salida de vehiculo.\n");
    printf("3. Calcular pago.\n");
    printf("4. Resporte del día.\n");
    printf("5. Estado de espacios.\n");
    printf("6. Salir.\n");
    printf("> Seleccione opcion: ");
    fgets(opcion, 3, stdin);
    eliminar_retorno_linea(opcion);
  }while(atoi(opcion) < 1 || atoi(opcion) > 6);
  return atoi(opcion);
}

int *
buscar_por_placa(Espacio_t *parqueadero, const unsigned int n_cantidad_datos, const char *placa)
{
	int i;
	for(int i = 0; i < n_cantidad_datos;i++)
	{
		if(strcmp(parqueadero[i].placa, placa) == 0)
			return i;
	}
	return -1;
}

void
registrar_vehiculo(Espacio_t *array, 
	unsigned int *cantidad_autos, unsigned int *cantidad_motos)
{
	system("clear");
	Espacio_t espacio;
	char tipo_vehiculo[2];
	int tv;
	printf("**** Registro de nuevo vehiculo *****\n\n");
	printf("Ingrese placa: ");
	fgets(espacio.placa, 9, stdin);
	eliminar_retorno_linea(espacio.placa);
	if(buscar_por_placa(array, *cantidad_autos + *cantidad_motos, espacio.placa) != -1)
	{
		printf("Ya se encuentra un vehiculo registrado con ese numero de placa\n");
		pausar_programa();
		return;
	}
	do{
		printf("Elija el tipo de vehiculo.\n");
		printf("1. Automovil\n");
		printf("2. Motocicleta\n");
		printf("> Seleccione una opcion: ");
		fgets(tipo_vehiculo, 3, stdin);
		eliminar_retorno_linea(tipo_vehiculo);
		tv = atoi(tipo_vehiculo);
	}while(tv < 1 || tv > 2);
	if(tv == 1) {
		if((*cantidad_autos + 1) > LEN_AUTOMOVILES) {
			printf("Ya se encuentra lleno el cupo para automoviles.\n");
			pausar_programa();
			return;
		}
		*cantidad_autos = *cantidad_autos + 1;
		espacio.tipo_vehiculo = AUTOMOVIL;
	} else {
		if((*cantidad_motos + 1) > LEN_MOTOCICLETAS) {
			printf("Ya se encuentra lleno el cupo para motocicletas.\n");
			pausar_programa();
			return;
		}
		*cantidad_motos = *cantidad_motos + 1;
		espacio.tipo_vehiculo = MOTOCICLETA;
	}
	printf("Ingrese color: ");
	fgets(espacio.color, 15, stdin);
	eliminar_retorno_linea(espacio.color);
	espacio.estado = OCUPADO;
	espacio.hora_ingreso = time(0);
	array[(*cantidad_motos + *cantidad_autos) - 1] = espacio;
	pausar_programa();
	FILE *f = fopen(FILE_PATH, "ab");
	if(f) {
		fseek(f, 0, SEEK_END);
		fwrite(&espacio, sizeof(Espacio_t), 1, f);
		printf("Vehiculo registrado correctamente\n");
		system("pause");
	}
	fclose(f);
}


/* registra la salida de un vehiculo */
void
salida_vehiculo(Espacio_t *array, unsigned int *ca, unsigned int *cm)
{
	printf("**** Salida de vehiculos *****\n\n");
	printf("Ingrese placa: ");
	Espacio_t espacio, to_read;
	int posicion_vehiculo;
	fgets(espacio.placa, 9, stdin);
	eliminar_retorno_linea(espacio.placa);
	posicion_vehiculo = buscar_por_placa(array, (*ca + *cm), espacio.placa);
	if(posicion_vehiculo != -1) {
		/* algoritmo para eliminar el vehiculo del arreglo */
		espacio = array[posicion_vehiculo];
		int i;
		for(i = posicion_vehiculo; i < (*ca +*cm) - 1; i++)
		{
			array[i] = array[i + 1];
		}
		double tiempo_en_minutos = calcular_tiempo_en_minutos(espacio);
		printf("Tiempo en minudos : %.3f\n", tiempo_en_minutos);
		if(espacio.tipo_vehiculo == AUTOMOVIL) {
			printf("Valor a pagar : $%.2f\n", tiempo_en_minutos * TARIFA_AUTOMOVILES);
			*ca = *ca - 1;
		} else {
			printf("Valor a pagar : %.2f\n", tiempo_en_minutos * TARIFA_MOTOCICLETAS);
			*cm = *cm - 1;
		}
		/* algoritmo para eliminar el vehiculo del archivo */
		FILE *f = fopen(FILE_PATH, "rb");
		FILE *ft = fopen(TEMP_FILE_PATH, "wb");
		fseek(f, 0, SEEK_SET);
		fseek(ft, 0, SEEK_SET);
		while(fread(&to_read, sizeof(Espacio_t), 1, f) == 1)
		{
			if(strcmp(espacio.placa, to_read.placa) != 0) {
				fwrite(&to_read, sizeof(Espacio_t), 1, ft);
			}
		}
		remove(FILE_PATH);
		rename(TEMP_FILE_PATH, FILE_PATH);
		fclose(f);
		fclose(ft);
		printf("Salida de vehiculo exitosa.\n");
		pausar_programa();
	} else {
		printf("No existe un vehiculo con ese número de placa registrado.\n");
		pausar_programa();
	}
}

void
calcular_pago(Espacio_t *array, const unsigned int n)
{
	printf("**** Calcular pago de vehiculos *****\n\n");
	Espacio_t espacio;
	int posicion_vehiculo;
	printf("Ingrese placa: ");
	fgets(espacio.placa, 9, stdin);
	eliminar_retorno_linea(espacio.placa);
	posicion_vehiculo = buscar_por_placa(array, n, espacio.placa);
	if(posicion_vehiculo != -1) {
		espacio = array[posicion_vehiculo];
		double tiempo_en_minutos = calcular_tiempo_en_minutos(espacio);
		printf("Tiempo en minudos : %.0f\n", tiempo_en_minutos);
		if(espacio.tipo_vehiculo == AUTOMOVIL) {
			printf("Valor a pagar : $%.2f\n", tiempo_en_minutos * TARIFA_AUTOMOVILES);
		} else {
			printf("Valor a pagar : %.2f\n", tiempo_en_minutos * TARIFA_MOTOCICLETAS);
		}
		pausar_programa();
	} else {
		printf("No existe un vehiculo con ese número de placa registrado.\n");
		pausar_programa();
	}
}


void
mostrar_estado_espacio(Espacio_t *a, const unsigned int ca, const unsigned int cm)
{
	printf("****** Estado de espacios ******\n\n");
	printf("Hay %d espacios opcupados.\n", (ca + cm));
	printf("Hay %d espacios disponibles para Automoviles.\n", (LEN_AUTOMOVILES - ca));
	printf("Hay %d espacios disponibles para Moticicletas.\n", (LEN_MOTOCICLETAS - cm));
	pausar_programa();
}

void
mostrar_reporte_dia(Espacio_t *a, const unsigned int n)
{
	printf("***** Reporte del dia *******\n\n");
	
	pausar_programa();
}
