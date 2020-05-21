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
/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */

/* ************************************************************************** */
struct tm pedirHora() {
    //En este objeto time vamos a poner el tiempo para pasarselo al RTC
    struct tm tiempo;
    uint8_t entrada[6];
    uint8_t horas = 25, minutos = 61, segundos = 61;


    while (horas > 25 || horas < 0 || minutos > 61 || minutos < 0 || segundos < 0 || segundos > 25) {
        buscarEntrada(entrada, 6); //Este metodo pone la entrada en el array pasado con el largo dado.
        segundos = ((entrada[5]) - 48) + (10 * (entrada[4] - 48));
        minutos = ((entrada[3]) - 48) + (10 * (entrada[2] - 48));
        horas = ((entrada[1]) - 48) + (10 * (entrada[0] - 48));
    }
    tiempo.tm_hour = horas;
    tiempo.tm_min = minutos;
    tiempo.tm_sec = segundos;

    return tiempo;
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
                mando =true;
            }
        }
    }
}


/* *****************************************************************************
 End of File
 */
