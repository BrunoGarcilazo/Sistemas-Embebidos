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

app_event_t eventos[EVENTOS_MAXIMOS];

void agregarEvento() {

    uint8_t command;
    uint8_t led;
    uint8_t color;
    time_t tiempoFinal;
    time_t tiempoInicial;

    uint8_t valorNumericoDeEntrada;

    enviarMensaje("\r\nIngrese 0 si desea prender 1 si desea apagar un LED\r\n");
    uint8_t entrada[1];
    valorNumericoDeEntrada = 2;
    while (valorNumericoDeEntrada != 1 && valorNumericoDeEntrada != 0) { //Si selecciona una opcion valida
        buscarEntrada(entrada, 1);
        valorNumericoDeEntrada = entrada[0] - 48;
    }
    command = valorNumericoDeEntrada;
    entrada[0] = 200;

    if (command == 1) { //Si se selecciono prender
        enviarMensaje("\r\n¿De que color desea encender el Led?\r\n0- Blanco\r\n1- Rojo\r\n2- Azul\r\n3- Verde\r\n");
        valorNumericoDeEntrada = 5;
        while (valorNumericoDeEntrada > 3 || valorNumericoDeEntrada < 0) { //Si selecciona algo no entre 0 y 3
            buscarEntrada(entrada, 1);
            valorNumericoDeEntrada = entrada[0] - 48;
        }
        color = valorNumericoDeEntrada;
        entrada[0] = 200;

        enviarMensaje("\r\n¿Que Led del 0 al 7 Desea Prender?\r\n");
    } else { //Si se selecciono apagar
        enviarMensaje("\r\n¿Que Led del 0 al 7 desea apagar?\r\n");
    }

    valorNumericoDeEntrada = 10;
    while (valorNumericoDeEntrada > 7 || valorNumericoDeEntrada < 0) {
        buscarEntrada(entrada, 1);
        valorNumericoDeEntrada = entrada[0] - 48;
    }
    led = valorNumericoDeEntrada;
    entrada[0] = 200;

    enviarMensaje("\r\nIngrese hora en formato hh:mm:ss\r\n");
    struct tm tiempo;
    RTCC_TimeGet(&tiempo);
    tiempoInicial = mktime(&tiempo);


    RTCC_TimeGet(&tiempo);
    pedirHora(&tiempo);
    tiempoFinal = mktime(&tiempo);



    app_event_t evento;
    evento.color = color;
    evento.command = command;
    evento.param = led;
    evento.time = tiempoFinal - tiempoInicial;

    int i;
    for (i = 0; i < EVENTOS_MAXIMOS - 1; i++) {
        if (eventos[i].command == 0xFF) {
            eventos[i] = evento;
            break;
        }
    }
    
}

void inicilizarEventos() {
    int i;
    for (i = 0; i < EVENTOS_MAXIMOS - 1; i++) {
        eventos[i].command = 0xFF;
    }
}

/* *****************************************************************************
 End of File
 */
