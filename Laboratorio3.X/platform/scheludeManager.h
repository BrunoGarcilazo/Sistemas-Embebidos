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

/* ************************************************************************** */
/* Section: Constantes                                                        */
/* ************************************************************************** */

#define EVENTOS_MAXIMOS 7


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

/**
 * Se usa para verificar en que estado se encuentra la aplicacion en el main
 */
typedef enum {
    /**
     * Cuando se esta requiriendo al usuario que ingrese un tiempo
     */
    WAITING_FOR_TIME_INPUT,

    /**
     * Refiere al estado normal donde el usuario debe pedir algo.
     */
    MENU_STATE,
} schelude_t;




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


#endif /* _SCHELUDE_MANAGER_H */

/* *****************************************************************************
 End of File
 */
