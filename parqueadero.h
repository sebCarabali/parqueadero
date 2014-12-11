#include <time.h>

#define LEN_AUTOMOVILES 30
#define LEN_MOTOCICLETAS 20
#define TARIFA_AUTOMOVILES 50
#define TARIFA_MOTOCICLETAS 30
#define FILE_PATH "data/parqueadero.dat"
#define TEMP_FILE_PATH "data/temp_parqueadero.dat"

/* estados de los puestos */
typedef enum { OCUPADO = 0, LIBRE } Estado_t;
/* tipos de automoviles */
typedef enum { AUTOMOVIL = 0, MOTOCICLETA } TipoVehiculo_t;

/* estructura para los datos*/
typedef struct {
  char placa[8];
  char color[14];
  time_t hora_ingreso;
  time_t hora_egreso;
  TipoVehiculo_t tipo_vehiculo;
  Estado_t estado;
} Espacio_t; 

/* carga los datos del archivo */
void cargar_datos(Espacio_t *, unsigned int*, unsigned int*);

/* muestra el menu principal de la aplicacion */
int menu_parqueadero();

/* busca un vehiculo por placa (retorna la posicion en el arreglo)*/
int *buscar_por_placa(Espacio_t *, const unsigned int , const char *);

/* captura la informacion de ingreso de un vehiculo */
void registrar_vehiculo(Espacio_t*, unsigned int*, unsigned int*);

/* registra la salida de un vehiculo */
void salida_vehiculo(Espacio_t*, unsigned int*, unsigned int*);

/* calcula el valor total a pagar de un carro sin realizar la salida del mismo */
void calcular_pago(Espacio_t*, const unsigned int);

/* muestra el reporte del dia */
void mostrar_reporte_dia(Espacio_t*, const unsigned int);

/* muestra el estado actual de los espacion en el parquedaero */
void mostrar_estado_espacio(Espacio_t *, const unsigned int, const unsigned int);
