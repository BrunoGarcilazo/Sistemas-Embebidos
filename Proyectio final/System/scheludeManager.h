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

/*Formatos de entrada o salida*/
#define FORMATO_DE_APAGAR "\r\nIngrese la informacion en el siguiente formato\r\nNumeroDeLED;\r\n"
#define FORMATO_PARA_PRENDER "\r\nIngrese la informacion en el siguiente formato\r\nColor,NumeroDeLED\r\n"
#define FORMATO_DE_AGENDA "\r\nCada linea representa un evento y tiene el siguiente formato:\r\n posicion, color, hora de ejecuccion\r\n"
#define FORMATO_QUITAR_EVENTO "\n\rIngrese la posicion del evento que desea quitar con este formato:\n\rPoscion\r\nLas posiciones van de 0 a 7\r\n"
#define FORMATO_DE_LISTA_DE_EVENTOS "Cada linea representa una tarea. Las lineas tienen el siguiente formato: \r\nMomento de Activacion,Posicion,Led,Color,Accion\r\n"

/*Preguntas*/
#define PREGUNTA_DE_COMAND "\r\nIngrese 1 si desea prender 0 si desea apagar un LED\r\n"
#define SELECCION_DE_LED "\r\n¿Que Led del 0 al 7 desea accionar?\r\n"
#define PREGUNTA_POR_COLOR_DE_LED "\r\n¿De que color desea encender el Led?\r\n0- Blanco\r\n1- Rojo\r\n2- Azul\r\n3- Verde\r\n"

/*Avisos*/
#define ELEMENTO_BORRADO "\r\nElemento Borrado Satisfactoriamente\r\n"

#define EVENTOS_MAXIMOS 7
#define ASCCI_TO_INT_DIFFERENCE 48

/*Array de eventos*/
app_event_t eventos[EVENTOS_MAXIMOS];

/*Tiempo que usa el sistema. Se actualiza cada un segundo*/
static struct tm tiempoDelSistema;

/*Informacion para prender los leds de colores*/
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