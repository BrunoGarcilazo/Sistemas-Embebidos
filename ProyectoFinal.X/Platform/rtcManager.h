/* ************************************************************************** */
/** En este archivo hay funciones que manejan el RTC en base a interaccion via USB

  @Company
    UCUDAL

  @File Name
    rtcManager.h

  @Summary
    Funciones que usan el RTC y USB

  @Description
    Procesar input del usuario e interactuar con el USB
 */
/* ************************************************************************** */

#ifndef _RTC_MANAGER_H    /* Guard against multiple inclusion */
#define _RTC_MANAGER_H

/* ************************************************************************** */
/* Section: Include                                                           */
/* ************************************************************************** */
#include <time.h>
#include "../freeRTOS/include/semphr.h"


/* ************************************************************************** */
/* Section: Constantes                                                        */
/* ************************************************************************** */
#define FORMATO_DE_HORA "\r\nIngrese hora en formato hh:mm:ss\r\n\0"
#define FORMATO_DE_FECHA "\r\nIngrese fecha en formato dd/mm/aaaa\r\n\0"

/*Tiempo que usa el sistema*/
static struct tm tiempoDelSistema;

/*Semaforo que indica que el sistema obtuvo hora del GPS*/
static SemaphoreHandle_t horaSeteada;
/*Semaforo que indica si la trama obtenida es valida*/
static SemaphoreHandle_t tramaValida;

/* ************************************************************************** */
/* Seccion: Funciones de interfaz                                             */
/* ************************************************************************** */
/**
 * Esta funcion pide al usuario que ingrese la fecha que sera usada para usar el RTC
 * Junto con la hora inicial.
 */
void inicializar(void);

/* ************************************************************************** */
/* Seccion: Funciones de interfaz                                             */
/* ************************************************************************** */

/**
 * Levanta una Hora ingresada por el usuario y la coloca en el struct tiempo
 * Imprime un mensaje con el formato requerido
 * @param tiempo struct tm donde se almacena lo ingresado 
 */
void pedirHora(struct tm *tiempo);

/**
 * Envia un mensaje por usb indicando el formato de fecha a introducir
 * Es bloqueante, esta función no termina hasta que la fecha es valida
 * @param tiempo struct donde se guardan los datos de fecha
 */
void pedirFecha(struct tm *tiempo);

/**
 * Funcion que envia la hora actual del Reloj RTC por USB
 */
void mostrarHora();

/**
 * Tarea que se encarga de refrescar los datos del GPS
 * @param p_params
 */
void mantenerGPS(void *p_params);

#endif /* _RTC_MANAGER_H */

/* *****************************************************************************
 End of File
 */
