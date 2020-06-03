/* ************************************************************************** */
/** Funciones que se encargan de manejar las actividades de la agenda

  @Company
    UCUDAL

  @File Name
    scheludeManager.h

  @Summary
    Funciones para gestionar los eventos, structs para guardar los estados de las funciones,
    string para comunicarse con el usuario

  @Description
    Ningunas de las funciones presentes son bloqueantes. Todas se encargan de gestionar
    eventos y pueden guardar su estado. Se incluyen los string con los cuales se va a 
    interactuar con el usuario
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

/**
 * Representacion del estado de la aplicacion
 */
typedef struct {
    APP_STATUS status;

} aplicacion_t;

/**
 * Estados de que puede tomar la funcion agregar enventos
 */
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

/*
 *Representa el estado de una adicion de un evento
 */
typedef struct {
    /**
     * Estados posibles de la funcion
     */
    ADD_EVENT_ADDER_STATUS estado;
    
    /**
     * Struct tm para procesar inputs
     */
    struct tm tiempo;
    
    /**
     * Command del evento
     */
    uint8_t command;
    
    /**
     * Numero de leds del evento
     */
    uint8_t led;
    
    /**
     * Color con el cual se debe prender el led
     */
    uint8_t color;
    
    /**
     * Segundos desde 1970 cuando se debe ejecutar el evento
     */
    time_t tiempoFinal;
    
} eventAdder_t;

/**
 * Estados que puede tomar una operacion de eliminacion de evento
 */
typedef enum {
    ENVIANDO_INSTRUCCIONES,
    RECIBIENDO_ENTRADA,
    BORRANDO_EVENTO,
    COMUNICANDO,
} ERASE_EVENT_KICKER_STATUS;

/**
 * Representa el estado de una ejecucion de eliminacion de evento
 */
typedef struct {
    /**
     * Poscicion del evento a borrar en el array de eventos
     */
    uint8_t posicion;
    
    /**
     * Estado de la operacion
     */
    ERASE_EVENT_KICKER_STATUS estado;
} event_kicker_t;

/**
 * Estado que puede tomar la operacion de emitir el listado de eventos
 */
typedef enum {
    ENVIANDO_FORMATO,
    ENVIANDO_POSICION,
    ENVIANDO_NUMERO_DE_LED,
    ENVIANDO_COLOR,
    ENVIANDO_TIEMPO,
    SUMAR_CONTADOR,
} CONSULTAR_EVENTOS_STATE;

/**
 * Estado que puede tomar una funcion de dar a conocer los eventos
 */
typedef struct {
    /**
     * Estado de la operacion
     */
    CONSULTAR_EVENTOS_STATE estado;
    
    /**
     * Contador que va a iterar sobre el array de eventos
     */
    uint8_t contador;
    
    /**
     * Struct tm tiempo para 
     */
    struct tm tiempo;
} event_voicer_t;

/* ************************************************************************** */
/* Section: Constantes                                                        */
/* ************************************************************************** */

//Formato de inputs
#define FORMATO_DE_HORA "\r\nIngrese hora en formato hh:mm:ss\r\n"
#define FORMATO_DE_FECHA "\r\nIngrese fecha en formato dd/mm/aaaa\r\n"
#define FORMATO_DE_APAGAR "\r\nIngrese la informacion en el siguiente formato\r\nNumeroDeLED;\r\n"
#define FORMATO_PARA_PRENDER "\r\nIngrese la informacion en el siguiente formato\r\nColor,NumeroDeLED\r\n"
#define FORMATO_DE_AGENDA "\r\nCada linea representa un evento y tiene el siguiente formato:\r\n posicion, color, hora de ejecuccion\r\n"
#define FORMATO_QUITAR_EVENTO "\n\rIngrese la posicion del evento que desea quitar con este formato\n\rPoscion.\r\nLas posiciones van de 0 a 7\r\n"



#define SELECCION_DE_LED "\r\n¿Que Led del 0 al 7 desea accionar?\r\n"
#define PREGUNTA_POR_COLOR_DE_LED "\r\n¿De que color desea encender el Led?\r\n0- Blanco\r\n1- Rojo\r\n2- Azul\r\n3- Verde\r\n"
#define PREGUNTA_DE_COMAND "\r\nIngrese 1 si desea prender 0 si desea apagar un LED\r\n"
#define ELEMENTO_BORRADO "\r\nElemento Borrado Satisfactoriamente\r\n"
#define MENU "\r\nBienvenido\r\nMenu\r\nPara seleccionar una opcion envie la tecla presente a la izquierda de la funcion\r\na - Fijar hora del reloj RTC\r\nb - Consultar hora del RTC\r\nc - Agregar evento\r\nd - Quitar evento\r\ne - Consultar lista de eventos\r\n"

#define EVENTOS_MAXIMOS 8
app_event_t eventos[EVENTOS_MAXIMOS];


/* ************************************************************************** */
/* Section: Funciones de interfaz                                             */
/* ************************************************************************** */


/**
 * Agrega un evento al array de eventos segun informacion pasada por usb
 * Tener especial cuidado con el formato que se pide a los inputs
 * @param adder struct que maneja el estado de la operacion
 * @param entrada Array para gestionar los input
 * @param manager Manager para ejecutar input de fecha y hora
 * @return Retorna true cuando se completo la operacion. False si sigue en curso
 */
bool agregarEvento(eventAdder_t *adder, uint8_t * entrada, manager_de_pedidos_t *manager);

/**
 * Inicializa los eventos del array para que notemos que no estan configurados por defecto
 */
void inicilizarEventos();

/**
 * Quita un evento de la posicion indicada por el operador via USB
 * @param kicker Struct que maneja el evento del parcial
 * @param entrada Array para gestionar el input del indice del evento 
 * @return true cuando se completo la operacion, false de lo contrario
 */
bool quitarEvento(event_kicker_t *kicker, uint8_t *entrada);

/**
 * Escribe en el menu la lista de eventos activos
 * @param voicer Struct que representa el estado de la operacion
 * @param salida Array para gestionar los outputs de la funcion
 * @return True cuando termino la operacion, false de lo contrario
 */
bool consultarListaDeEventos(event_voicer_t * voicer, char * salida);


#endif /* _SCHELUDE_MANAGER_H */

/* *****************************************************************************
 End of File
 */
