/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _SCHELUDE_MANAGER_H    /* Guard against multiple inclusion */
#define _SCHELUDE_MANAGER_H


/* ************************************************************************** */
/* Section: Include                                                           */
/* ************************************************************************** */
#include<stdint.h>
#include<time.h>
#include"../Platform/WS2812.h"




/* ************************************************************************** */
/* Section: Tipos de dato                                                     */

/* ************************************************************************** */


typedef struct {
    /**
     * //Accion de realizar evento. 1 prender, 0 apagar, 0xFF si no esta configurado
     */
    uint8_t command;

    /**
     * Led que se debe prender o apagar
     */
    uint8_t param;

    /**
     * De que color encender un led
     * 0 es blanco
     * 1 es rojo
     * 2 es azul
     * 3 es verde
     */
    uint8_t color;

    /**
     * Momento en que se tiene que disparar el evento       
     */
    uint32_t time;

} app_event_t;


/* ************************************************************************** */
/* Section: Constantes                                                        */
/* ************************************************************************** */

#define EVENTOS_MAXIMOS 7
app_event_t eventos[EVENTOS_MAXIMOS];
static struct tm tiempoDelSistema;
static ws2812_t ledsRGB[8];


/* ************************************************************************** */
/* Section: Funciones de interfaz                                             */
/* ************************************************************************** */

/**
 * Agrega un evento al calendario segun informacion del operador recibida por USB
 */
void agregarEvento();
/**
 * Inicializa los eventos del array para que notemos que no estan configurados
 */
void inicilizarEventos();

/**
 * Quita un evento de la posicion indicada por el operador
 * Esta posicion se obtiene dentro de la funcion
 */
void quitarEvento();

/**
 * Escribe en el menu la lista de eventos activos
 */
void consultarListaDeEventos();

/**
 * Esta tarea se encarga de verificar cada un segundo si llego el tiempo de ejecutar un evento
 * @param p_param
 */
void verificarEventos(void *p_param);

/**
 * Muestra la hora actual del sistema
 */
void mostrarHora();

#endif /* _SCHELUDE_MANAGER_H */

/* *****************************************************************************
 End of File
 */