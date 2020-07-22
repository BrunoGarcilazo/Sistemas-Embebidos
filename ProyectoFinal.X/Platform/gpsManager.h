/** En este archivo hay funciones que se encargar de la gestion de la trama GPS

  @Company
    UCUDAL

  @File Name
    gpsManager.h

  @Summary
    Tareas que se encargan de manejar los datos obtenidos por el gps

  @Description
    Libera un semaforo cuando la trama es valida. Permite a otras tareas usar datos del GPS
 */

#ifndef _GPS_MANAGER_H    /* Guard against multiple inclusion */
#define _GPS_MANAGER_H

// <editor-fold defaultstate="collapsed" desc="Includes">

//FreeRtos
#include "semphr.h"

//Librerias
#include <time.h>
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Datos Globales">

/*Tiempo que usa el sistema, se actualiza cada un seg*/
struct tm tiempoDelSistema;

/*Semaforo que indica si la trama obtenida es valida*/
extern SemaphoreHandle_t tramaValida;
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Tareas">
/*
 * Tarea que se encarga de refrescar los datos del GPS y la hora
 * @param p_params
 */
void mantenerGPS(void *p_params);
// </editor-fold>
#endif