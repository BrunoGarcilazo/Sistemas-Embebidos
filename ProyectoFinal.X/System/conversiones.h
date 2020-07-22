/** Conversiones

  @Company
    UCUDAL

  @File Name
    conversiones.h

  @Summary
    Se encarga de las mediciones de temperatura del dispositivo

  @Description
    Mide, promedia, convierte la medida a grados. Prende leds cuando esta midiendo y luego leds de colores 
    segun la salud del paciente.
 */

#ifndef _CONVERSIONES_H
#define _CONVERSIONES_H

// <editor-fold defaultstate="collapsed" desc="Includes">
//FreeRtos
#include "FreeRTOS.h"
#include "semphr.h"

//Librerias
#include <stdint.h>
#include <time.h>

//Archivos del sistema
#include "../Communications/GPS.h"
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Tipos de datos">
/**
 * Tipo de dato encargado de representar una medida tomada por el conversion AD ("termometro")
 */
typedef struct{
    float temperaturaRegistrada; //Temperatura registrada en decimal, por la forma de calculo siempre tendra 1 decimal
    time_t tiempo; //Tiempo medido en segundos desde 1970
    GPSPosition_t posicion; //Posicion donde se tomo la medida
} medida_t ;
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Datos Globales">
/**
 * Array donde se guardan las mediciones tomadas por el "termometro"
 */
medida_t mediciones[200];

/**
 * Indice donde se va a agregar la proxima medida en el array de mediciones
 */
uint8_t ultimaMedida;

/**
 * Semaforo que dice a la tarea de hacer parpadear la luz roja por 2 seg
 */
extern SemaphoreHandle_t prenderYapagarLucesRojas;

/**
 * Semaforo que dice a la tarea de hacer parpadear la luz verde por 2 seg
 */
extern SemaphoreHandle_t prenderYapagarLucesVerdes;
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Funciones de interfaz">
/**
 * Tarea que se encarga de realizar las conversiones AD. Una conversion solo se hace cuando los semaforos de trama
 * valida, de incializacion y de permiso para medir de la interfaz estan habilitados. El semaforo de permiso
 * de la interfaz se chequea antes de realizar cada parte de la operacion por si el usuario paro la conversion
 * @param p_params
 */
void conversiones(void *p_params);
// </editor-fold>

#endif
