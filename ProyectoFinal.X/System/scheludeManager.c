/* ************************************************************************** */
/** Funciones encargadas de operar con la agenda

  @Company
 UCUDAL

  @File Name
    scheludeManager.c

  @Summary
    Tarea que checkea eventos. Funciones para agregar, inicializar y quitar eventos.
    Funciones para consultar lista de eventos y mostrar la hora del sistema

  @Description
    Se encarga de gestionar los eventos de la agenda
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/*RTOS Includes*/
#include "FreeRTOS.h"
#include "task.h"

/*MCC includes*/
#include "../mcc_generated_files/system.h"
#include "../mcc_generated_files/pin_manager.h"


/* Libraries */
#include <stdint.h>
#include <time.h>
#include <string.h>

#include "../Platform/WS2812.h"
#include "../Platform/usbManager.h"
#include "scheludeManager.h"

void verificarEventos(void *p_param) {
    uint8_t q;
    uint32_t tiempoActualPlano;
    int i;
    //La primera vez que se ejecuta se apagan todos los leds
    for (q = 0; q < sizeof (ledsRGB); q++) {
        ledsRGB[q] = BLACK;
    }
    while (1) {

        for (i = 0; i < EVENTOS_MAXIMOS - 1; i++) {
            if (eventos[i].command != 0xFF) { //Si un evento esta configurado
                tiempoActualPlano = mktime(&tiempoDelSistema); //Se pasa el tiempo del sistema a segundos despues de 1970
                if (tiempoActualPlano >= eventos[i].time) { //Si ya paso el momento de activacion
                    if (eventos[i].command == 0) { //Se apaga el led que se debe apagar
                        ledsRGB[eventos[i].param] = BLACK;
                    } else { //Si se debe prender un led
                        //Se prende un led del color
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
                    //Se pone el evento como no configurado
                    //Esto hace que esa posicion del array pueda ser sobreescrita con otro evento
                    eventos[i].command = 0xFF;
                }
            }
        }
        //Se actualiza el tiempo del sistema
        RTCC_TimeGet(&tiempoDelSistema);
        //Se prenden los respectivos led
        WS2812_send(ledsRGB, sizeof (ledsRGB));
        //Se espera un segundo
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void agregarEvento() {
    uint8_t command;
    uint8_t led;
    uint8_t color;
    time_t tiempoFinal;
    uint8_t valorNumericoDeEntrada;
    uint8_t entrada[1];
    app_event_t evento;
    int i;
    struct tm tiempo;

    enviarMensaje(PREGUNTA_DE_COMAND); //Se pregunta al usuario si quiere prender o apagar un led
    do {
        buscarEntrada(entrada, sizeof (entrada)); //Se actualiza la entrada
        valorNumericoDeEntrada = entrada[0] - ASCCI_TO_INT_DIFFERENCE; //Se cambia al valor numerico
    } while (valorNumericoDeEntrada != 1 && valorNumericoDeEntrada != 0); //Si no se selecciona una opcion valida

    command = valorNumericoDeEntrada;

    if (command == 1) { //Si se selecciono prender
        enviarMensaje(PREGUNTA_POR_COLOR_DE_LED); //Se pregunta de que color se desea prender el led
        do {
            buscarEntrada(entrada, sizeof (entrada)); //Actualizamos la entrada
            valorNumericoDeEntrada = entrada[0] - ASCCI_TO_INT_DIFFERENCE; //Se pone en valor numerico
        } while (valorNumericoDeEntrada > 3 || valorNumericoDeEntrada < 0); //Si no selecciona algo fuera de 0 y 3

        color = valorNumericoDeEntrada;
    }

    //Se pregunta la usuario que led desea accionar
    enviarMensaje(SELECCION_DE_LED);

    do {
        buscarEntrada(entrada, sizeof (entrada));
        valorNumericoDeEntrada = entrada[0] - ASCCI_TO_INT_DIFFERENCE;
    } while (valorNumericoDeEntrada > 7 || valorNumericoDeEntrada < 0);

    led = valorNumericoDeEntrada;

    //Pedimos hora y fecha
    pedirHora(&tiempo);
    pedirFecha(&tiempo);

    //Creamos el evento
    tiempoFinal = mktime(&tiempo);

    evento.color = color;
    evento.command = command;
    evento.param = led;
    evento.time = tiempoFinal;

    //El evento se agrega en el primer lugar disponible
    for (i = 0; i < EVENTOS_MAXIMOS - 1; i++) {
        if (eventos[i].command == 0xFF) {
            eventos[i] = evento;
            break;
        }
    }
}

void inicilizarEventos() {
    //Se inicializan todos los eventos a no configurado
    int i;
    for (i = 0; i < EVENTOS_MAXIMOS - 1; i++) {
        eventos[i].command = 0xFF;
    }
}

void quitarEvento() {
    //Pide al usuario que ingrese la posicion del evento que desea quitar
    //Esta se puede obtener imprimiendo la lista de eventos
    enviarMensaje(FORMATO_QUITAR_EVENTO);
    uint8_t entrada[2];
    uint8_t valorNumerico;
    app_event_t *eventoABorrar;

    do {
        buscarEntrada(entrada, sizeof (entrada));
        valorNumerico = entrada[0] - ASCCI_TO_INT_DIFFERENCE;

    } while (valorNumerico > 7 || valorNumerico < 0); //Si el numero es valido


    /*Se setea el evento a no configurado y se notifica. Si en la posicion del evento habia un evento de prender luz
    leds se apaga el led que estaba prendido*/
    eventoABorrar = &eventos[valorNumerico];
    eventoABorrar->command = 0xFF;
    ledsRGB[eventoABorrar->param] = BLACK;
    enviarMensaje(ELEMENTO_BORRADO);
}

void consultarListaDeEventos() {
    int i;
    struct tm *horaDeActivacion;
    char datos[28];
    char salida[189];
    time_t horaDeFinalizacion;

    memset(salida, 0, sizeof (salida));
    enviarMensaje(FORMATO_DE_LISTA_DE_EVENTOS); //Se comunica el formato de las lineas a imprimir

    //Para cada evento se imprime: momento de activacion,posicion,Led,Color,accion
    for (i = 0; i < EVENTOS_MAXIMOS - 1; i++) {
        if (eventos[i].command != 0xFF) {

            horaDeFinalizacion = eventos[i].time;

            horaDeActivacion = gmtime(&horaDeFinalizacion);

            //Setear Tiempo
            strftime(datos, 19, "%d/%m/%Y - %H:%M,", horaDeActivacion);

            //Setear posicion. Poner 48 en define.
            datos[19] = i + ASCCI_TO_INT_DIFFERENCE;
            datos[20] = ',';

            //Setear Numero de Led
            datos[21] = (eventos[i].param + ASCCI_TO_INT_DIFFERENCE);
            datos[22] = ',';

            //Setear accion
            datos[25] = (eventos[i].command + ASCCI_TO_INT_DIFFERENCE);
            datos[26] = '\r';
            datos[27] = '\n';

            //Setear Color
            if (eventos[i].command == 1) {
                datos[23] = (eventos[i].color + ASCCI_TO_INT_DIFFERENCE);
            } else {
                datos[23] = '_';
            }

            datos[24] = ',';

            strcat(salida, datos);
        }
    }
    enviarMensaje(salida);
}

void mostrarHora() {
    //Muestra la hora del sistema.
    char hora[23];
    strftime(hora, sizeof (hora), "%d/%m/%Y-%H:%M\0", &tiempoDelSistema);
    enviarMensaje(hora);
}
/* *****************************************************************************
 End of File
 */
