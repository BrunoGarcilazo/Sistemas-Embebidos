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
#include <stdint.h>
#include "../platform/rtcManager.h"



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
     * Indica si la fecha y hora fueron inicializadas
     */
    NO_INICIALIZADA,

    /**
     * Refiere al estado normal donde el usuario debe pedir algo.
     */
    EN_MENU,

    /**
     * 
     */
    EN_MOSTRAR_HORA,

    /**
     * Cuando se esta requiriendo al usuario que ingrese un tiempo
     */
    EN_INGRESAR_HORA,

    /**
     * Cuando el usuario esta agregando un evento
     */
    EN_AGREGAR_EVENTO,

    /**
     * Cuando el usuario este quitando un evento
     */
    EN_QUITAR_EVENTO,

    /**
     * Cuando el usuario este consultando la lista de eventos
     */
    EN_CONSULTAR_LISTA_DE_EVENTOS,

    /**
     * Cuando el sistema no ha recibido nada
     */
    EN_ESPERA,

} APP_STATUS;

typedef struct {
    APP_STATUS status;

} aplicacion_t;

typedef enum {
    ENVIANDO_MENSAJE_DE_COMAND,
    RECIBIENDO_COMAND,
    ENVIANDO_MENSAJE_DE_COLOR,
    RECIBIENDO_COLOR,
    ENVIANDO_MENSAJE_DE_PARAM,
    RECIBIENDO_PARAM,
    ENVIANDO_MENSAJE_DE_HORA,
    RECIBIENDO_HORA,
    ENVIANDO_MENSAJE_DE_FECHA,
    RECIBIENDO_FECHA,
    CREANDO_EVENTO,
    COLOCANDO_EN_LISTA,
} ADD_EVENT_ADDER_STATUS;

typedef struct {
    ADD_EVENT_ADDER_STATUS estado;
    struct tm tiempo;
    uint8_t command;
    uint8_t led;
    uint8_t color;
    time_t tiempoFinal;
    uint8_t valorNumericoDeEntrada;
} eventAdder_t;

/* ************************************************************************** */
/* Section: Constantes                                                        */
/* ************************************************************************** */

#define EVENTOS_MAXIMOS 7
app_event_t eventos[EVENTOS_MAXIMOS];


/* ************************************************************************** */
/* Section: Funciones de interfaz                                             */
/* ************************************************************************** */

/**
 * Agrega un evento al calendario segun informacion del operador recibida por USB
 */
bool agregarEvento(eventAdder_t *adder, uint8_t * entrada, manager_de_pedidos_t *manager);

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


#endif /* _SCHELUDE_MANAGER_H */

/* *****************************************************************************
 End of File
 */
