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
#include <stdint.h>
#include <time.h>
#include <proc/PIC32MM-GPM-0XX/p32mm0256gpm064.h>
#include "../platform/usbManager.h"
#include "../platform/scheludeManager.h"
#include "rtcManager.h"

bool agregarEvento(eventAdder_t *adder, uint8_t * entrada, manager_de_pedidos_t *manager) {
    switch (adder -> estado) {
        case(ENVIANDO_MENSAJE_DE_COMAND):
            if (enviarMensaje("\r\nIngrese 0 si desea prender 1 si desea apagar un LED\r\n")) {
                adder -> estado = RECIBIENDO_COMAND;
            }
            break;
        case(RECIBIENDO_COMAND):
            if (buscarEntrada(entrada, 1)) {
                if (entrada[0] == '1' || entrada[0] == '0') {
                    adder -> command = entrada[0] - 48;
                    if (adder -> command == 1) {
                        adder -> estado = ENVIANDO_MENSAJE_PARA_APAGAR_INSTRUCCIONES;
                    } else {
                        adder -> estado = ENVIANDO_MENSAJE_PARA_PRENDER_INSTRUCCIONES;
                    }
                }
            }
            break;
        case(ENVIANDO_MENSAJE_PARA_PRENDER_INSTRUCCIONES):
            if (enviarMensaje("\r\nIngrese la informacion en el siguiente formato\r\nColor,NumeroDeLED\r\n")) {
                adder -> estado = ENVIANDO_MENSAJE_PARA_PRENDER_COLORES;
            }
            break;
        case (ENVIANDO_MENSAJE_PARA_PRENDER_COLORES):
            if (enviarMensaje("\r\n¿De que color desea encender el Led?\r\n0- Blanco\r\n1- Rojo\r\n2- Azul\r\n3- Verde\r\n")) {
                adder -> estado = ENVIANDO_MENSAJE_DE_PARAM;
            }
            break;
        case(ENVIANDO_MENSAJE_DE_PARAM):
            if (enviarMensaje("\r\n¿Que Led del 0 al 7 desea accionar?\r\n")) {
                if (adder -> command == 0) {
                    adder -> estado = RECIBIENDO_INFORMACION_DE_PRENDER;
                } else {
                    adder -> estado = RECIBIENDO_INFORMACION_DE_APAGAR;
                }

            }
            break;
        case(RECIBIENDO_INFORMACION_DE_PRENDER):
            if (buscarEntrada(entrada, 3)) {
                if (!(entrada[0] < '0' || entrada[0] > '3') && entrada[1] == ',' && (entrada[2] < '8' && entrada[2] >= '0')) {
                    adder -> color = (entrada[0] - 48);
                    adder -> led = (entrada[2] - 48);
                    adder -> estado = ENVIANDO_MENSAJE_DE_HORA;
                }
            }
            break;
        case(ENVIANDO_MENSAJE_PARA_APAGAR_INSTRUCCIONES):
            if (enviarMensaje("\r\nIngrese la informacion en el siguiente formato\r\nNumeroDeLED;\r\n")) {
                adder -> estado = ENVIANDO_MENSAJE_DE_PARAM;
            }
            break;
        case(RECIBIENDO_INFORMACION_DE_APAGAR):
            if (buscarEntrada(entrada, 2)) {
                if ((entrada[0] < '8' && entrada[0] >= '0') && entrada[1] == ';') {
                    adder -> led = (entrada[2] - 48);
                    adder -> estado = ENVIANDO_MENSAJE_DE_HORA;
                }
            }
            break;
        case(ENVIANDO_MENSAJE_DE_HORA):
            if (enviarMensaje("\r\nIngrese hora en formato hh:mm:ss\r\n")) {
                adder -> estado = RECIBIENDO_HORA;
            }
            break;
        case(RECIBIENDO_HORA):
            if (pedirHora(&(adder -> tiempo), manager)) {
                adder -> estado = ENVIANDO_MENSAJE_DE_FECHA;
            }
            break;
        case(ENVIANDO_MENSAJE_DE_FECHA):
            if (enviarMensaje("\r\nIngrese fecha en formato dd/mm/aaaa\r\n")) {
                adder -> estado = RECIBIENDO_FECHA;
            }
            break;
        case(RECIBIENDO_FECHA):
            if (pedirFecha(&(adder -> tiempo), manager)) {
                adder -> estado = CREANDO_EVENTO;
            }
            break;
        case(CREANDO_EVENTO):
        {
            app_event_t evento;
            uint32_t tiempoFinal;
            tiempoFinal = mktime(&(adder -> tiempo));
            evento.color = adder -> color;
            evento.command = adder -> command;
            evento.param = adder -> led;
            evento.time = tiempoFinal;

            int i;
            for (i = 0; i < EVENTOS_MAXIMOS - 1; i++) {
                if (eventos[i].command == 0xFF) {
                    eventos[i] = evento;
                    break;
                }
            }
            return true;
        }

    }
    return false;
}

void inicilizarEventos() {
    int i;
    for (i = 0; i < EVENTOS_MAXIMOS - 1; i++) {

        eventos[i].command = 0xFF;
    }
}

void quitarEvento() {
    enviarMensaje("\n\rSeleccione la posicion del evento que desea quitar\r\n");
    enviarMensaje("\n\rLas posiciones van de 1 a 7\r\n");
    uint8_t entrada[1];
    entrada[1] = 8;
    uint8_t valorNumerico;
    valorNumerico = 97;
    while (valorNumerico > 7 || valorNumerico < 1) {
        buscarEntrada(entrada, 1);
        valorNumerico = entrada[0] - 48;
    }
    valorNumerico--;
    app_event_t eventoABorrar;
    eventoABorrar = eventos[valorNumerico];
    if (eventoABorrar.command != 0xFF) {

        eventoABorrar.command = 0xFF;
        enviarMensaje("\r\nElemento Borrado Satisfactoriamente");
    }
}

void consultarListaDeEventos() {
    int i;

    struct tm *horaDeActivacion;

    for (i = 0; i < EVENTOS_MAXIMOS - 1; i++) {
        if (eventos[i].command != 0xFF) {
            char posicion[1];
            char param[1];
            char color[1];
            char dias[1];
            char minutos[1];
            char segundos[1];
            time_t horaDeFinalizacion = eventos[i].time;
            horaDeActivacion = gmtime(&horaDeFinalizacion);

            //Enviar Posicion
            enviarMensaje("\r\nPosicion: ");
            posicion[0] = i + 48;
            enviarMensaje(posicion);

            //Enviar Numero de Led
            enviarMensaje(" \r\n");
            enviarMensaje("Led: ");
            param[0] = (eventos[i].param + 48);
            enviarMensaje(param);


            //Enviar Color
            enviarMensaje("\r\n");
            enviarMensaje("Color: ");
            color[0] = (eventos[i].color + 48);
            enviarMensaje(color);

            //Enviar Tiempo
            enviarMensaje("\r\n");
            enviarMensaje("Momento de activacion: ");
            dias[0] = (horaDeActivacion->tm_mday + 48);
            enviarMensaje(dias);
            enviarMensaje(":");
            minutos[0] = (horaDeActivacion->tm_min + 48);
            enviarMensaje(minutos);
            enviarMensaje(":");
            segundos[0] = (horaDeActivacion->tm_sec + 48);
            enviarMensaje(segundos);
            enviarMensaje("\r\n");
        }
    }

}


/* *****************************************************************************
 End of File
 */
