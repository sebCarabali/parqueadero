#include "parqueadero.h"

int
main(int argc, char *argv[])
{
  /* opcion seleccionada por el usuario en el menu principal */
  unsigned int opt_menu_principal;
  /* cantidad de automoviles en el parqueadero */
  unsigned int cantidad_automoviles;
  /* cantidad de motocicletas en el parqueadero */
  unsigned int cantidad_moticicletas;  
  /* espacio para los puestos del automovil */
  Espacio_t parqueadero[LEN_AUTOMOVILES + LEN_MOTOCICLETAS];
  /* se inicializan los contadores en 0 */
  cantidad_automoviles = 0;
  cantidad_moticicletas = 0;
  /* cargando datos del archivo */
  cargar_datos(parqueadero, &cantidad_automoviles, &cantidad_moticicletas);
  
  /* ciclo del programa */
  do{
    opt_menu_principal = menu_parqueadero();
    switch(opt_menu_principal)
    {
    case 1:
		registrar_vehiculo(parqueadero, &cantidad_automoviles, &cantidad_moticicletas);
      break;
    case 2:
		salida_vehiculo(parqueadero, &cantidad_automoviles, &cantidad_moticicletas);
      break;
    case 3:
		calcular_pago(parqueadero, cantidad_automoviles + cantidad_moticicletas);
      break;
    case 4:
		mostrar_reporte_dia(parqueadero, cantidad_automoviles + cantidad_moticicletas);
      break;
    case 5:
		mostrar_estado_espacio(parqueadero, cantidad_automoviles, cantidad_moticicletas);
      break;
    case 6:
      printf("\n\n ****** Gracias por uilizar el programa.***** \n");
      return 0;
      break;
    default:
      printf("Opcion no valida\n");
    }
  }while(1);
  return 0;
}
