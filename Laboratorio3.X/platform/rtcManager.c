/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include <time.h>
#include <stdbool.h>
#include "../mcc_generated_files/usb/usb_device_cdc.h"
#include "../mcc_generated_files/rtcc.h"
#include "../utilss/utils.h"
#include "rtcManager.h"
/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */

/* ************************************************************************** */
bool pedirHora(struct tm *tiempo, manager_de_pedidos_t *estado_de_pedido) {
    switch (estado_de_pedido-> estado) {
        case(PEDIDO_INVALIDO):
        {
            uint8_t segundos;
            uint8_t minutos;
            uint8_t horas;
            uint8_t entrada[10];
            buscarEntrada(entrada, 8); //Este metodo pone la entrada en el array pasado con el largo dado.
            segundos = ((entrada[7]) - 48) + (10 * (entrada[6] - 48));
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
        case(PEDIDO_VALIDO):
            tiempo->tm_hour = estado_de_pedido -> masSignificativo;
            tiempo->tm_min = estado_de_pedido -> medioSignificativo;
            tiempo->tm_sec = estado_de_pedido -> menosSignificativo;
            estado_de_pedido -> estado = PEDIDO_INVALIDO;
            return true;
    }
}

bool consultarHora() {
    uint8_t hora[12];
    hora[0] = '\r';
    hora[1] = '\n';
    hora[2] = RTCTIMEbits.HRTEN + 48;
    hora[3] = RTCTIMEbits.HRONE + 48;
    hora[4] = ':';
    hora[5] = RTCTIMEbits.MINTEN + 48;
    hora[6] = RTCTIMEbits.MINONE + 48;
    hora[7] = ':';
    hora[8] = RTCTIMEbits.SECTEN + 48;
    hora[9] = RTCTIMEbits.SECONE + 48;
    hora[10] = '\r';
    hora[11] = '\n';

    CDCTxService();
    if ((USBGetDeviceState() < CONFIGURED_STATE) ||
            (USBIsDeviceSuspended() == true)) {
        return false;
    } else {
        if (USBUSARTIsTxTrfReady()) {
            putsUSBUSART(hora);
            return true;
        }
    }
    return false;
}

bool inicializarFechaYHora(inicializador_t * inicializador, manager_de_pedidos_t *managerDePedidos) {
    switch (inicializador -> estado) {
        case(MENSAJE_DE_FECHA_NO_ENVIADO):
            if (enviarMensaje("\r\nIngrese fecha en formato dd/mm/aaaa\r\n")) {
                inicializador -> estado = ESPERANDO_FECHA;
            }
            break;
        case(ESPERANDO_FECHA):
            if (pedirFecha(&(inicializador -> tiempo), managerDePedidos)) {
                inicializador -> estado = MENSAJE_DE_HORA_NO_ENVIADO;
            }
            break;
        case(MENSAJE_DE_HORA_NO_ENVIADO):
            if (enviarMensaje("\r\nIngrese hora en formato hh:mm:ss\r\n")) {
                inicializador -> estado = ESPERANDO_HORA;
            }
            break;
        case(ESPERANDO_HORA):
            if (pedirHora(&(inicializador -> tiempo), managerDePedidos)) {
                inicializador -> estado = SETEAR_TIEMPO;
            }
            break;
        case(SETEAR_TIEMPO):
            RTCC_TimeSet(&(inicializador -> tiempo));
            return true;
    }
    return false;
}

bool pedirFecha(struct tm * tiempo, manager_de_pedidos_t *estado_de_pedido) {
    switch (estado_de_pedido -> estado) {
        case(PEDIDO_INVALIDO):
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
                estado_de_pedido -> menosSignificativo = dia;
                estado_de_pedido -> medioSignificativo = mes;
                estado_de_pedido -> masSignificativo = year;
                estado_de_pedido -> estado = PEDIDO_VALIDO;
            }
            return false;
        }
        case(PEDIDO_VALIDO):
            tiempo->tm_year = (estado_de_pedido -> masSignificativo) - 1900;
            tiempo->tm_mon = estado_de_pedido -> medioSignificativo;
            tiempo->tm_mday = estado_de_pedido -> menosSignificativo;
            estado_de_pedido -> estado = PEDIDO_INVALIDO;
            return true;
    }
}

/* *****************************************************************************
 End of File
 */
