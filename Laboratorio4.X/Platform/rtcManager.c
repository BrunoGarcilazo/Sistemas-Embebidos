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
#include "usbManager.h"
#include "../System/Menu.h"

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */

/* ************************************************************************** */
void inicializar(void *p_params) {
    struct tm tiempo;
    pedirHora(&tiempo);
    pedirFecha(&tiempo);
    RTCC_TimeSet(&tiempo);
    vTaskDelete(NULL);
}

void pedirHora(struct tm* tiempo) {
    enviarMensaje(FORMATO_DE_HORA);
    uint8_t entrada[8];
    uint8_t horas = 25, minutos = 61, segundos = 61;
    while ((horas > 23 || horas < 0 || minutos > 59 || minutos < 0 || segundos < 0 || segundos > 59)
            || entrada[2] != ':' || entrada[5] != ':') {
        buscarEntrada(entrada, 8); //Este metodo pone la entrada en el array pasado con el largo dado.
        segundos = ((entrada[7]) - 48) + (10 * (entrada[6] - 48));
        minutos = ((entrada[4]) - 48) + (10 * (entrada[3] - 48));
        horas = ((entrada[1]) - 48) + (10 * (entrada[0] - 48));
    }
    tiempo->tm_hour = horas;
    tiempo->tm_min = minutos;
    tiempo->tm_sec = segundos;
}

void pedirFecha(struct tm* tiempo) {
    enviarMensaje(FORMATO_DE_FECHA);
    uint8_t fecha[10];
    uint8_t dia = 250;
    uint8_t mes = 250;
    uint16_t year = 1000;
    while ((dia > 31 || dia < 1 || mes < 1 || mes > 31 || year < 1970) || fecha[2] != '/' || fecha[5] != '/') {
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
