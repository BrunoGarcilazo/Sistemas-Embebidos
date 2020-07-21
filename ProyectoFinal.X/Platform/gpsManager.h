/* ************************************************************************** */
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
/* ************************************************************************** */

#ifndef _GPS_MANAGER_H    /* Guard against multiple inclusion */
#define _GPS_MANAGER_H

/* ************************************************************************** */
/* Section: Include                                                           */
/* ************************************************************************** */
#include <time.h>
#include "../freeRTOS/include/semphr.h"

/*Tiempo que usa el sistema*/
struct tm tiempoDelSistema;

/*Semaforo que indica si la trama obtenida es valida*/
extern SemaphoreHandle_t tramaValida;

/* ************************************************************************** */
/* Seccion: Funciones de interfaz                                             */
/* ************************************************************************** */

/*
 * Tarea que se encarga de refrescar los datos del GPS
 * @param p_params
 */
void mantenerGPS(void *p_params);

#endif /* _GPS_MANAGER_H */