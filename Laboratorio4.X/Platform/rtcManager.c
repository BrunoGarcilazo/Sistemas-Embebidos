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

/*Libraries*/
#include <time.h>
#include <stdbool.h>
#include <string.h>

/*MCC Includes*/
#include "../mcc_generated_files/rtcc.h"

#include "rtcManager.h"
#include "usbManager.h"
#include "../System/menu.h"
#include "../System/scheludeManager.h"

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

void inicializar() {
    struct tm tiempo;
    pedirHora(&tiempo);
    pedirFecha(&tiempo);
    RTCC_TimeSet(&tiempo);
}

void pedirHora(struct tm* tiempo) {
    enviarMensaje(FORMATO_DE_HORA);//Comunica el formato en que se debe imprimir la hora
    uint8_t entrada[8]; //Array donde se va a recibir la entrada
    uint8_t horas, minutos, segundos;
    do {
        memset(entrada,0,sizeof(entrada)); //Se limpia la entrada
        buscarEntrada(entrada, sizeof(entrada)); //Este metodo pone la entrada USB en el array pasado con el largo dado.
        //Se pasan los caracteres a numeros y se calcula el valor numerico de hora, minutos y segundos
        segundos = ((entrada[7]) - '0') + (10 * (entrada[6] - '0')); 
        minutos = ((entrada[4]) - '0') + (10 * (entrada[3] - '0'));
        horas = ((entrada[1]) - '0') + (10 * (entrada[0] - '0'));
    } while ((horas > 23 || horas < 0 || minutos > 59 || minutos < 0 || segundos < 0 || segundos > 59)
            || entrada[2] != ':' || entrada[5] != ':'); //Mientras que los minutos, horas y segundos no esten en rango
    tiempo->tm_hour = horas;
    tiempo->tm_min = minutos;
    tiempo->tm_sec = segundos;
}

void pedirFecha(struct tm* tiempo) {
    enviarMensaje(FORMATO_DE_FECHA);//Se comunica en que formato se debe ingresar la fecha
    uint8_t fecha[10];
    uint8_t dia;
    uint8_t mes;
    uint16_t year;
    do {
        memset(fecha, 0, sizeof (fecha)); //Se limpia la entrada
        buscarEntrada(fecha, sizeof (fecha)); //Se busca una nueva entrada
        dia = (fecha[1] - '0') + 10 * (fecha[0] - '0');
        mes = (fecha[4] - '0') + 10 * (fecha[3] - '0');
        year = ((fecha[6] - '0') * 1000) + (100 * (fecha[7] - '0')) + (10 * (fecha[8] - '0')) + (fecha[9] - '0');
    } while ((dia > 31 || dia < 1 || mes < 1 || mes > 31 || year < 1970) || fecha[2] != '/' || fecha[5] != '/');
    tiempo->tm_year = year - 1900;
    tiempo->tm_mon = mes - 1;
    tiempo->tm_mday = dia;
}

/* *****************************************************************************
 End of File
 */
