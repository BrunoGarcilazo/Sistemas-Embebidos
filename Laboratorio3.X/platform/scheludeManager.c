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
            if (enviarMensaje(PREGUNTA_DE_COMAND)) {
                adder -> estado = RECIBIENDO_COMAND;
            }
            break;
        case(RECIBIENDO_COMAND):
            if (buscarEntrada(entrada, 1)) {
                if (entrada[0] == '1' || entrada[0] == '0') {
                    adder -> command = entrada[0] - 48;
                    if (adder -> command == 0) {
                        adder -> estado = ENVIANDO_MENSAJE_PARA_APAGAR_INSTRUCCIONES;
                    } else {
                        adder -> estado = ENVIANDO_MENSAJE_PARA_PRENDER_INSTRUCCIONES;
                    }
                }
            }
            break;
        case(ENVIANDO_MENSAJE_PARA_PRENDER_INSTRUCCIONES):
            if (enviarMensaje(FORMATO_PARA_PRENDER)) {
                adder -> estado = ENVIANDO_MENSAJE_PARA_PRENDER_COLORES;
            }
            break;
        case (ENVIANDO_MENSAJE_PARA_PRENDER_COLORES):
            if (enviarMensaje(PREGUNTA_POR_COLOR_DE_LED)) {
                adder -> estado = ENVIANDO_MENSAJE_DE_PARAM;
            }
            break;
        case(ENVIANDO_MENSAJE_DE_PARAM):
            if (enviarMensaje(SELECCION_DE_LED)) {
                if (adder -> command == 1) {
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
            if (enviarMensaje(FORMATO_DE_PRENDER)) {
                adder -> estado = ENVIANDO_MENSAJE_DE_PARAM;
            }
            break;
        case(RECIBIENDO_INFORMACION_DE_APAGAR):
            if (buscarEntrada(entrada, 2)) {
                if ((entrada[0] < '8' && entrada[0] >= '0') && entrada[1] == ';') {
                    adder -> led = (entrada[0] - 48);
                    adder -> estado = ENVIANDO_MENSAJE_DE_HORA;
                }
            }
            break;
        case(ENVIANDO_MENSAJE_DE_HORA):
            if (enviarMensaje(FORMATO_DE_HORA)) {
                adder -> estado = RECIBIENDO_HORA;
            }
            break;
        case(RECIBIENDO_HORA):
            if (pedirHora(&(adder -> tiempo), manager)) {
                adder -> estado = ENVIANDO_MENSAJE_DE_FECHA;
            }
            break;
        case(ENVIANDO_MENSAJE_DE_FECHA):
            if (enviarMensaje(FORMATO_DE_FECHA)) {
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

            eventos[evento.param] = evento;
            return true;
        }

    }
    return false;
}

void inicilizarEventos() {
    int i;
    for (i = 0; i < EVENTOS_MAXIMOS; i++) {
        eventos[i].command = 0xFF;
    }
}

bool quitarEvento(event_kicker_t *kicker, uint8_t *entrada) {
    switch (kicker -> estado) {
        case(ENVIANDO_INSTRUCCIONES):
            if (enviarMensaje(FORMATO_QUITAR_EVENTO)) {
                kicker -> estado = RECIBIENDO_ENTRADA;
            }
            break;
        case(RECIBIENDO_ENTRADA):
            if (buscarEntrada(entrada, 2)) {
                if (entrada[0] < '8' && entrada [0] >= '0' && entrada[1] == '.') {
                    kicker -> estado = BORRANDO_EVENTO;
                    kicker -> posicion = (entrada[0] - 48);
                }
            }
            break;
        case(BORRANDO_EVENTO):
        {
            app_event_t *eventoABorrar;
            eventoABorrar = &eventos[kicker -> posicion];
            if (eventoABorrar->command != 0xFF) {
                eventoABorrar->command = 0xFF;
            }
            kicker -> estado = COMUNICANDO;
            break;
        }
        case (COMUNICANDO):
            enviarMensaje(ELEMENTO_BORRADO);
            return true;
    }
    return false;
}

bool consultarListaDeEventos(event_voicer_t * voicer, char * salida) {
    switch (voicer -> estado) {
        case(SUMAR_CONTADOR):
            if (voicer -> contador >= EVENTOS_MAXIMOS) {
                return true;
            } else {
                (voicer -> contador)++;
                if (eventos[(voicer -> contador)].command == 0xFF) {
                    voicer -> estado = SUMAR_CONTADOR;
                } else {
                    voicer -> estado = ENVIANDO_POSICION;
                }
            }
            return false;
        case(ENVIANDO_FORMATO):
            if (enviarMensaje(FORMATO_DE_AGENDA)) {
                if (eventos[voicer -> contador].command == 0xFF) {
                    voicer -> estado = SUMAR_CONTADOR;
                } else {
                    voicer -> estado = ENVIANDO_POSICION;
                }
            }
            return false;
        case(ENVIANDO_POSICION):
            salida[0] = voicer -> contador + 48;
            salida[1] = ',';
            salida[2] = ' ';
            if (enviarMensaje(salida)) {
                voicer -> estado = ENVIANDO_COLOR;
            }
            return false;
        case(ENVIANDO_COLOR):
        {
            switch (eventos[voicer -> contador].color) {
                case(0):
                    salida = "BLANCO, ";
                    break;
                case(1):
                    salida = "ROJO, ";
                    break;
                case(2):
                    salida = "AZUL, ";
                    break;
                case(3):
                    salida = "VERDE, ";
                    break;
                default:
                    salida = "....";
                    break;
            }
            if (enviarMensaje(salida)) {
                voicer -> estado = ENVIANDO_TIEMPO;
            }
            return false;
        }
        case(ENVIANDO_TIEMPO):
        {
            struct tm *tiempoFormateado;
            
            char tiempo[7];
            time_t tiempoPlano;
            
            tiempoPlano = eventos[voicer -> contador].time;
            tiempoFormateado = gmtime(&tiempoPlano);
            
            strftime(tiempo, 7, "%H:%M\r\n",tiempoFormateado);
            if (enviarMensaje(tiempo)) {
                voicer -> estado = SUMAR_CONTADOR;
            }
            break;
        }

        default:
            return false;
    }
    return false;
}



/* *****************************************************************************
 End of File
 */
