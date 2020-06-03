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

/* Kernel includes*/
#include "FreeRTOS.h"
#include "task.h"
#include "../mcc_generated_files/system.h"
#include "../mcc_generated_files/pin_manager.h"
#include "../System/menu.h"
/* */
#include <stdint.h>
#include <time.h>
#include "../Platform/WS2812.h"
#include <proc/PIC32MM-GPM-0XX/p32mm0256gpm064.h>
#include "../Platform/usbManager.h"
#include "scheludeManager.h"

void verificarEventos(void *p_param) {
    uint8_t q;
    for (q = 0; q < 8; q++) {
        ledsRGB[q] = BLACK;
    }
    while (1) {
        int i;
        for (i = 0; i < EVENTOS_MAXIMOS - 1; i++) {
            if (eventos[i].command != 0xFF) {
                uint32_t tiempoActualPlano = mktime(&tiempoDelSistema);
                if (tiempoActualPlano >= eventos[i].time) {
                    if (eventos[i].command == 0) {
                        ledsRGB[eventos[i].param] = BLACK;
                    } else {
                        switch (eventos[i].color) {
                            case 0:
                                ledsRGB[eventos[i].param] = WHITE;
                                break;
                            case 1:
                                ledsRGB[eventos[i].param] = RED;
                                break;
                            case 2:
                                ledsRGB[eventos[i].param] = BLUE;
                                break;
                            case 3:
                                ledsRGB[eventos[i].param] = GREEN;
                            default:
                                break;
                        }
                    }
                    eventos[i].command = 0xFF;
                }
            }
        }
        RTCC_TimeGet(&tiempoDelSistema);
        WS2812_send(ledsRGB, 7);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void agregarEvento() {
    uint8_t command;
    uint8_t led;
    uint8_t color;
    time_t tiempoFinal;
    uint8_t valorNumericoDeEntrada;

    enviarMensaje(PREGUNTA_DE_COMAND);
    uint8_t entrada[1];
    valorNumericoDeEntrada = 2;
    while (valorNumericoDeEntrada != 1 && valorNumericoDeEntrada != 0) { //Si no se selecciona una opcion valida
        buscarEntrada(entrada, 1);
        valorNumericoDeEntrada = entrada[0] - 48;
    }
    command = valorNumericoDeEntrada;
    entrada[0] = 200;

    if (command == 1) { //Si se selecciono prender
        enviarMensaje(PREGUNTA_POR_COLOR_DE_LED);
        valorNumericoDeEntrada = 5;
        while (valorNumericoDeEntrada > 3 || valorNumericoDeEntrada < 0) { //Si no selecciona algo no entre 0 y 3
            buscarEntrada(entrada, 1);
            valorNumericoDeEntrada = entrada[0] - 48;
        }
        color = valorNumericoDeEntrada;
        entrada[0] = 200;

    }
    enviarMensaje(SELECCION_DE_LED);
    valorNumericoDeEntrada = 10;
    while (valorNumericoDeEntrada > 7 || valorNumericoDeEntrada < 0) {
        buscarEntrada(entrada, 1);
        valorNumericoDeEntrada = entrada[0] - 48;
    }
    led = valorNumericoDeEntrada;
    entrada[0] = 200;

    struct tm tiempo;
    pedirHora(&tiempo);
    pedirFecha(&tiempo);
    tiempoFinal = mktime(&tiempo);

    app_event_t evento;
    evento.color = color;
    evento.command = command;
    evento.param = led;
    evento.time = tiempoFinal;

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

void quitarEvento() {
    enviarMensaje(FORMATO_QUITAR_EVENTO);
    uint8_t entrada[2];
    entrada[1] = 8;
    uint8_t valorNumerico;
    valorNumerico = 97;
    while (valorNumerico > 7 || valorNumerico < 0 || entrada[1] != '.') {
        buscarEntrada(entrada, 2);
        valorNumerico = entrada[0] - 48;
    }
    app_event_t *eventoABorrar;
    eventoABorrar = &eventos[valorNumerico];
    eventoABorrar->command = 0xFF;
    ledsRGB[eventoABorrar->param] = BLACK;
    enviarMensaje(ELEMENTO_BORRADO);
}

void consultarListaDeEventos() {
    int i;

    struct tm *horaDeActivacion;

    for (i = 0; i < EVENTOS_MAXIMOS - 1; i++) {
        if (eventos[i].command != 0xFF) {
            char posicion[1];
            char param[1];
            char color[1];
            char hora[30];
            time_t horaDeFinalizacion = eventos[i].time;
            horaDeActivacion = gmtime(&horaDeFinalizacion);

            //Enviar Posicion
            enviarMensaje("\r\nPosicion: ");
            posicion[0] = i + 48;
            enviarMensaje(posicion);

            //Enviar Numero de Led
            enviarMensaje("\r\nLed: ");
            param[0] = (eventos[i].param + 48);
            enviarMensaje(param);


            //Enviar Color
            enviarMensaje("\r\nColor: ");
            color[0] = (eventos[i].color + 48);
            enviarMensaje(color);

            //Enviar Tiempo
            enviarMensaje("\r\nMomento de activacion: ");
            strftime(hora, 30, "%d/%m/%Y - %H:%M \r\n", horaDeActivacion);
            hora[4] = hora[4] - 1;
            enviarMensaje(hora);
        }
    }

}

void mostrarHora() {
    char hora[10];
    strftime(hora, 10, "%H:%M", &tiempoDelSistema);
    enviarMensaje(hora);
}
/* *****************************************************************************
 End of File
 */
