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
#include "scheludeManager.h"

/* ************************************************************************** */
/* Section: Tipos de dato                                                     */

/* ************************************************************************** */


typedef enum {
    MENSAJE_DE_FECHA_NO_ENVIADO,
    ESPERANDO_FECHA,
    MENSAJE_DE_HORA_NO_ENVIADO,
    ESPERANDO_HORA,
    SETEAR_TIEMPO,
} ESTADO_DE_INICIALIZACION ;

typedef struct {
    ESTADO_DE_INICIALIZACION estado;
    struct tm tiempo;
} inicializador_t;

typedef enum {
    PEDIDO_VALIDO,
    PEDIDO_INVALIDO,
} ESTADO_DE_PEDIDO;

typedef struct {
    uint8_t entrada[10];
    ESTADO_DE_PEDIDO estado;
    uint8_t menosSignificativo;
    uint8_t medioSignificativo;
    uint16_t masSignificativo;
} manager_de_pedidos_t ;

/* ************************************************************************** */
/* Section: Funciones de interfaz                                             */
/* ************************************************************************** */

/**
 * Le pide hora al operador via USB y la coloca en el RTC
 * El formato de hora debe ser Hora-Minutos-Segundos (sin caracteres en medio)
 */
bool pedirFecha(struct tm * tiempo, manager_de_pedidos_t *estado_de_pedido);

/**
 * Imprime en la consola el tiempo actual via USB
 */
bool consultarHora(void);

/**
 * Esta funcion pide al usuario que ingrese la fecha que sera usada para usar el RTC
 * Junto con la hora inicial
 */
bool inicializarFechaYHora(inicializador_t * inicializador, manager_de_pedidos_t *managerDePedidos);

/**
 * Pide al usuario que ingrese una fecha
 * @param time struct tm que guardará los datos
 */
bool pedirHora(struct tm *tiempo, manager_de_pedidos_t *estado_de_pedido);

#endif /* _RTC_MANAGER_H */

/* *****************************************************************************
 End of File
 */
