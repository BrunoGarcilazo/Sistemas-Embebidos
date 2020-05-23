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
//#include "scheludeManager.h"

/* ************************************************************************** */
/* Section: Tipos de dato                                                     */

/* ************************************************************************** */

/**
 * Estados que se usan para inicializar la fecha y la hora del sistema 
 */
typedef enum {
    MENSAJE_DE_FECHA_NO_ENVIADO,
    ESPERANDO_FECHA,
    MENSAJE_DE_HORA_NO_ENVIADO,
    ESPERANDO_HORA,
    SETEAR_TIEMPO,
} ESTADO_DE_INICIALIZACION;

/**
 * struct donde se guarda el estado de una inicializacion
 */
typedef struct {
    ESTADO_DE_INICIALIZACION estado;
    struct tm tiempo;
} inicializador_t;

/**
 * Estados de un pedido de entrada por USB. Cuando se llame a una funcion de pedido
 * Si el pedido es invalido pide datos y verifica si son validos.
 * Cuando sean validos cambia el estado y se procesa la informacion
 */
typedef enum {
    PEDIDO_VALIDO,
    PEDIDO_INVALIDO,
} ESTADO_DE_PEDIDO;

/**
 * Se guarda el estado de las funciones de pedidos, tanto de fecha como de hora.
 */
typedef struct {
    /**
     * Array para ingresar datos
     */
    uint8_t entrada[10];

    /**
     * Estado del pedido
     */
    ESTADO_DE_PEDIDO estado;

    /**
     * Cifras menos significativas (segundos, dias)
     */
    uint8_t menosSignificativo;

    /**
     * Cifras de mitar de dato, son minutos o mes.
     */
    uint8_t medioSignificativo;

    /**
     * Cifras mas significativas, horas o años. Como recibe años es un int16
     */
    uint16_t masSignificativo;
} manager_de_pedidos_t;

/* ************************************************************************** */
/* Section: Funciones de interfaz                                             */
/* ************************************************************************** */


/**
 * Le pide hora al operador via USB y la coloca en el RTC
 * El formato de hora debe ser Dia/Mes/Año
 * @param tiempo struct tm de tiempo donde se van a procesar los datos conseguidos
 * @param estado_de_pedido manager de pedidos para implementacion de estados
 * @return Retorna true si se encontro una fecha valida y se guarda en tiempo
 */
bool pedirFecha(struct tm * tiempo, manager_de_pedidos_t *estado_de_pedido);


/**
 * El tiempo actual se guarda en el struct tm, se transforma a string con una funcion de la libreria
 * y si se pudo enviar devuelve tru
 * @param tiempo Se obtiene el tiempo actual en el RTC aqui
 * @return True si se pudo enviar, falso de lo contrario
 */
bool consultarHora(struct tm * tiempo);


/**
 * Esta funcion pide al usuario que ingrese la fecha que sera usada para usar el RTC
 * Junto con la hora inicial
 * @param inicializador Aqui se guarda el estado de la inicializacion
 * @param managerDePedidos Va a ser el encargado de manejar los estados del pedido de fecha y hora
 * @return Retorna true si termino
 */
bool inicializarFechaYHora(inicializador_t * inicializador, manager_de_pedidos_t *managerDePedidos);


/**
 * Levanta una  fecha ingresada por el usuario y la coloca en el struct tiempo
 * @param tiempo struct tm donde se almacena lo ingresado
 * @param estado_de_pedido Estado del pedido segun la validez de la entrada
 * @return True cuando se detecto una hora valida y se guardo en tiempo
 */
bool pedirHora(struct tm *tiempo, manager_de_pedidos_t *estado_de_pedido);

#endif /* _RTC_MANAGER_H */

/* *****************************************************************************
 End of File
 */
