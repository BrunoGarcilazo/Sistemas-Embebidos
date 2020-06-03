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
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "usbManager.h"
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
void pedirHora(struct tm *tiempo) {
    //En este objeto time vamos a poner el tiempo para pasarselo al RTC
    uint8_t entrada[8];
    uint8_t horas = 25, minutos = 61, segundos = 61;
    while (horas > 25 || horas < 0 || minutos > 61 || minutos < 0 || segundos < 0 || segundos > 60) {
        buscarEntrada(entrada, 8); //Este metodo pone la entrada en el array pasado con el largo dado.
        segundos = ((entrada[7]) - 48) + (10 * (entrada[6] - 48));
        minutos = ((entrada[4]) - 48) + (10 * (entrada[3] - 48));
        horas = ((entrada[1]) - 48) + (10 * (entrada[0] - 48));
    }
    tiempo->tm_hour = horas;
    tiempo->tm_min = minutos;
    tiempo->tm_sec = segundos;
}

void consultarHora() {
    char hora[8];
    hora[0] = RTCTIMEbits.HRTEN + 48;
    hora[1] = RTCTIMEbits.HRONE + 48;
    hora[2] = RTCTIMEbits.MINTEN + 48;
    hora[3] = RTCTIMEbits.MINONE + 48;
    hora[4] = RTCTIMEbits.SECTEN + 48;
    hora[5] = RTCTIMEbits.SECONE + 48;
    hora[6] = '\r';
    hora[7] = '\n';

    char hola[] = "hola";
    int contador;
    contador = 1;
    bool mando;
    mando = false;
    while (!mando) {
        CDCTxService();
        if ((USBGetDeviceState() < CONFIGURED_STATE) ||
                (USBIsDeviceSuspended() == true)) {
            continue;
        } else {
            if (USBUSARTIsTxTrfReady()) {
                putsUSBUSART(hora);
                mando = true;
            }
        }
    }
}

struct tm inicializarFechaYHora() {
    struct tm time;
    enviarMensaje("\r\nIngrese fecha en formato dd/mm/aaaa\r\n");
    pedirFecha(&time);

    enviarMensaje("\r\nIngrese hora en formato hh:mm:ss\r\n");
    pedirHora(&time);

    RTCC_TimeSet(&time);

    return time;
}

void pedirFecha(struct tm * tiempo) {
    uint8_t fecha[10];
    uint8_t dia = 32;
    uint8_t mes = 32;
    uint16_t year = 32;
    while (dia > 31 || dia < 1 || mes < 1 || mes > 31 || year < 1970) {
        buscarEntrada(fecha, 10);
        dia = (fecha[1] - 48) + 10 * (fecha[0] - 48);
        mes = (fecha[4] - 48) + 10 * (fecha[3] - 48);
        year = ((fecha[6] - 48) * 1000) + (100 * (fecha[7] - 48)) + (10 * (fecha[8] - 48)) + (fecha[9] - 48);
    }
    tiempo->tm_year = year - 1900;
    tiempo->tm_mon = mes;
    tiempo->tm_mday = dia;
}

/* *****************************************************************************
 End of File
 */
