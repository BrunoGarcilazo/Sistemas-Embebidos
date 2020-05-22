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
    ENVIANDO_MENSAJE_PARA_PRENDER_INSTRUCCIONES,
    ENVIANDO_MENSAJE_PARA_APAGAR_INSTRUCCIONES,
    ENVIANDO_MENSAJE_DE_PARAM,
    ENVIANDO_MENSAJE_PARA_PRENDER_COLORES,
    RECIBIENDO_INFORMACION_DE_PRENDER,
    ENVIANDO_MENSAJE_PARA_APAGAR,
    RECIBIENDO_INFORMACION_DE_APAGAR,
    ENVIANDO_MENSAJE_DE_HORA,
    ENVIANDO_MENSAJE_DE_FECHA,
    RECIBIENDO_HORA,
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

typedef enum {
    ENVIANDO_INSTRUCCIONES,
    RECIBIENDO_ENTRADA,
    BORRANDO_EVENTO,
    COMUNICANDO,
} ERASE_EVENT_KICKER_STATUS;

typedef struct {
    uint8_t posicion;
    ERASE_EVENT_KICKER_STATUS estado;
} event_kicker_t;

typedef enum {
    ENVIANDO_FORMATO,
    ENVIANDO_POSICION,
    ENVIANDO_NUMERO_DE_LED,
    ENVIANDO_COLOR,
    ENVIANDO_TIEMPO,
    SUMAR_CONTADOR,
}CONSULTAR_EVENTOS_STATE;

typedef struct{
    CONSULTAR_EVENTOS_STATE estado;
    uint8_t contador;
    struct tm tiempo;    
} event_voicer_t;
/* ************************************************************************** */
/* Section: Constantes                                                        */
/* ************************************************************************** */

#define SELECCION_DE_LED "\r\n¿Que Led del 0 al 7 desea accionar?\r\n"
#define FORMATO_DE_PRENDER "\r\nIngrese la informacion en el siguiente formato\r\nNumeroDeLED;\r\n"
#define PREGUNTA_POR_COLOR_DE_LED "\r\n¿De que color desea encender el Led?\r\n0- Blanco\r\n1- Rojo\r\n2- Azul\r\n3- Verde\r\n"
#define FORMATO_PARA_PRENDER "\r\nIngrese la informacion en el siguiente formato\r\nColor,NumeroDeLED\r\n"
#define PREGUNTA_DE_COMAND "\r\nIngrese 1 si desea prender 0 si desea apagar un LED\r\n"
#define FORMATO_DE_HORA "\r\nIngrese hora en formato hh:mm:ss\r\n"
#define FORMATO_DE_FECHA "\r\nIngrese fecha en formato dd/mm/aaaa\r\n"
#define FORMATO_QUITAR_EVENTO "\n\rIngrese la posicion del evento que desea quitar con este formato\n\r poscion.\r\nLas posiciones van de 0 a 7\r\n"
#define ELEMENTO_BORRADO "\r\nElemento Borrado Satisfactoriamente\r\n"
#define MENU "\r\nBienvenido\r\nMenu\r\nPara seleccionar una opcion envie la tecla presente a la izquierda de la funcion\r\na - Fijar hora del reloj RTC\r\nb - Consultar hora del RTC\r\nc - Agregar evento\r\nd - Quitar evento\r\ne - Consultar lista de eventos\r\n"
#define FORMATO_DE_AGENDA "\r\nCada linea representa un evento y tiene el siguiente formato: posicion, color, hora de ejecuccion\r\n"

#define EVENTOS_MAXIMOS 8
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
bool quitarEvento(event_kicker_t *kicker, uint8_t *entrada);

/**
 * Escribe en el menu la lista de eventos activos
 */
bool consultarListaDeEventos(event_voicer_t * voicer, char * salida);


#endif /* _SCHELUDE_MANAGER_H */

/* *****************************************************************************
 End of File
 */
