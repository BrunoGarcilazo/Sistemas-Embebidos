/* ************************************************************************** */
/** En este archivo hay funciones que manejan el RTC en base a interaccion via USB

  @Company
    UCUDAL

  @File Name
    rtcManager.c

  @Summary
    Funciones que usan el RTC y USB

  @Description
    Procesar input del usuario e interactuar con el USB
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include <time.h>
#include <stdbool.h>
#include "../mcc_generated_files/rtcc.h"
#include "rtcManager.h"
#include "scheludeManager.h"
#include "usbManager.h"
/* This section lists the other files that are included in this file.
 */


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

bool pedirHora(struct tm *tiempo, manager_de_pedidos_t *estado_de_pedido) {
    switch (estado_de_pedido-> estado) {
        case(PEDIDO_INVALIDO): //Estado donde se verifica si la entrada es valida
        {
            uint8_t segundos;
            uint8_t minutos;
            uint8_t horas;
            uint8_t entrada[10];
            buscarEntrada(entrada, 8); //Este metodo pone la entrada en el array pasado con el largo dado.
            segundos = ((entrada[7]) - 48) + (10 * (entrada[6] - 48)); //Se convierten los chars a su valor en int
            minutos = ((entrada[4]) - 48) + (10 * (entrada[3] - 48));
            horas = ((entrada[1]) - 48) + (10 * (entrada[0] - 48));
            if (!(horas > 25 || horas < 0 || minutos > 61 || minutos < 0 || segundos < 0 || segundos > 60)
                    && entrada[2] == ':' && entrada[5] == ':') {
                estado_de_pedido -> menosSignificativo = segundos;
                estado_de_pedido -> medioSignificativo = minutos;
                estado_de_pedido -> masSignificativo = horas;
                estado_de_pedido -> estado = PEDIDO_VALIDO;
            }
            return false;
        }
        case(PEDIDO_VALIDO): //Estado donde se guarda la entrada 
            tiempo->tm_hour = estado_de_pedido -> masSignificativo;
            tiempo->tm_min = estado_de_pedido -> medioSignificativo;
            tiempo->tm_sec = estado_de_pedido -> menosSignificativo;
            estado_de_pedido -> estado = PEDIDO_INVALIDO; //la proxima vez que se use el manager de pedidos debe estar asi
            return true;
    }
}


bool consultarHora(struct tm * tiempo) {
    RTCC_TimeGet(tiempo); //Se toma el tiempo actual
    char hora[5];
    strftime(hora, 5,"%H:%M",tiempo); //Esta funcion da formato a un struct tm
    return enviarMensaje(hora); //Retorna si se pudo enviar
}

bool inicializarFechaYHora(inicializador_t * inicializador, manager_de_pedidos_t *managerDePedidos) {
    switch (inicializador -> estado) {
        case(MENSAJE_DE_FECHA_NO_ENVIADO): //Envia el formato de fecha al usuario
            if (enviarMensaje(FORMATO_DE_FECHA)) {
                inicializador -> estado = ESPERANDO_FECHA;
            }
            break;
        case(ESPERANDO_FECHA): //Cuando se envie una fecha valida cambia de estado
            if (pedirFecha(&(inicializador -> tiempo), managerDePedidos)) {
                inicializador -> estado = MENSAJE_DE_HORA_NO_ENVIADO;
            }
            break;
        case(MENSAJE_DE_HORA_NO_ENVIADO): //Analogo a la fecha pero con hora
            if (enviarMensaje(FORMATO_DE_HORA)) {
                inicializador -> estado = ESPERANDO_HORA;
            }
            break;
        case(ESPERANDO_HORA):
            if (pedirHora(&(inicializador -> tiempo), managerDePedidos)) {
                inicializador -> estado = SETEAR_TIEMPO;
            }
            break;
        case(SETEAR_TIEMPO): //Setea el tiempo actual del 
            RTCC_TimeSet(&(inicializador -> tiempo));
            return true;
    }
    return false;
}

bool pedirFecha(struct tm * tiempo, manager_de_pedidos_t *estado_de_pedido) {
    switch (estado_de_pedido -> estado) {
        case(PEDIDO_INVALIDO): //Si el input no fue valido se chequea de nuevo
        {
            uint8_t dia;
            uint8_t mes;
            uint16_t year;
            uint8_t fecha[10];
            buscarEntrada(fecha, 10);
            dia = (fecha[1] - 48) + 10 * (fecha[0] - 48);
            mes = (fecha[4] - 48) + 10 * (fecha[3] - 48);
            year = ((fecha[6] - 48) * 1000) + (100 * (fecha[7] - 48)) + (10 * (fecha[8] - 48)) + (fecha[9] - 48);
            if (!(dia > 31 || dia < 1 || mes < 1 || mes > 31 || year < 1970) && fecha[2] == '/' && fecha[5] == '/') {
                //Si el input es valido se cambia el estado
                estado_de_pedido -> menosSignificativo = dia;
                estado_de_pedido -> medioSignificativo = mes;
                estado_de_pedido -> masSignificativo = year;
                estado_de_pedido -> estado = PEDIDO_VALIDO;
            }
            return false;
        }
        case(PEDIDO_VALIDO): //Con el input valido se guardan los datos
            tiempo->tm_year = (estado_de_pedido -> masSignificativo) - 1900; //El año de referencia es 1900, este fue un error comun
            tiempo->tm_mon = estado_de_pedido -> medioSignificativo;
            tiempo->tm_mday = estado_de_pedido -> menosSignificativo;
            estado_de_pedido -> estado = PEDIDO_INVALIDO; //Para poder reutilizar el manager de pedidos se hace esto
            return true; //Operacion exitosa
    }
}

/* *****************************************************************************
 End of File
 */
