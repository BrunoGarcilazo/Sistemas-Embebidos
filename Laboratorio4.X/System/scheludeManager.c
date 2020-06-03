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
#include "../System/Menu.h"
/* */
#include <stdint.h>
#include <time.h>
#include "../Platform/WS2812.h"
#include <proc/PIC32MM-GPM-0XX/p32mm0256gpm064.h>
#include "../Platform/usbManager.h"
#include "scheludeManager.h"

void verificarEventos(){
    while(1){
        struct tm tiempoActual;
            int i;
            ws2812_t ledsRGB[8];
            for (i = 0; i < EVENTOS_MAXIMOS - 1; i++){
                if (eventos[i].command != 0xFF){
                    RTCC_TimeGet(&tiempoActual);
                    uint32_t tiempoActualPlano = mktime(&tiempoActual);
                    if (eventos[i].time >= tiempoActualPlano){

                        switch(eventos[i].color){
                            case 0:
                              ledsRGB[eventos[i].param] =  WHITE;
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
                }
            }
            WS2812_send(ledsRGB,8);
            vTaskDelay(pdMS_TO_TICKS(750));
    }
}

void agregarEvento() {
    uint8_t command;
    uint8_t led;
    uint8_t color;
    time_t tiempoFinal;
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
    pedirHora(&tiempo);

    enviarMensaje("\r\nIngrese fecha en formato dd/mm/aaaa\r\n");
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
            enviarMensaje("\r\nSu tarea se puso en la posición" + (i + 48));
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
