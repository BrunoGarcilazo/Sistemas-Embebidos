/* ************************************************************************** */
/** Funciones que se encargan de manejar las actividades de la agenda

  @Company
    UCUDAL

  @File Name
    scheludeManager.c

  @Summary
    Funciones para gestionar los eventos, structs para guardar los estados de las funciones,
    string para comunicarse con el usuario

  @Description
    Ningunas de las funciones presentes son bloqueantes. Todas se encargan de gestionar
    eventos y pueden guardar su estado. Se incluyen los string con los cuales se va a 
    interactuar con el usuario
 */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include <stdint.h>
#include <time.h>
#include "../platform/usbManager.h"
#include "../platform/scheludeManager.h"
#include "rtcManager.h"

bool agregarEvento(eventAdder_t *adder, uint8_t * entrada, manager_de_pedidos_t *manager) {
    switch (adder -> estado) {
        case(ENVIANDO_MENSAJE_DE_COMAND): //Se envia informacion al usuario
            if (enviarMensaje(PREGUNTA_DE_COMAND)) { //Si se hizo el envio cambia de estado
                adder -> estado = RECIBIENDO_COMAND;
            }
            break;
        case(RECIBIENDO_COMAND): //Se recibe la informacion 
            if (buscarEntrada(entrada, 1)) {
                if (entrada[0] == '1' || entrada[0] == '0') { //De ser valida dse almacena y cambia el estado segun 
                    adder -> command = entrada[0] - 48;       //Se quiera apagar o prender un led
                    if (adder -> command == 0) {
                        adder -> estado = ENVIANDO_MENSAJE_PARA_APAGAR_INSTRUCCIONES;
                    } else {
                        adder -> estado = ENVIANDO_MENSAJE_PARA_PRENDER_INSTRUCCIONES;
                    }
                }
            }
            break;
        case(ENVIANDO_MENSAJE_PARA_PRENDER_INSTRUCCIONES)://Envia informacion al usuario
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
                if (adder -> command == 1) { //Segun se requiere informacion para prender o apagar se cambia el estado
                    adder -> estado = RECIBIENDO_INFORMACION_DE_PRENDER;
                } else {
                    adder -> estado = RECIBIENDO_INFORMACION_DE_APAGAR;
                }

            }
            break;
        case(RECIBIENDO_INFORMACION_DE_PRENDER):
            if (buscarEntrada(entrada, 3)) { //Recibe informacion sobre el color y cual led prender
                if (!(entrada[0] < '0' || entrada[0] > '3') && entrada[1] == ',' && (entrada[2] < '8' && entrada[2] >= '0')) {
                    adder -> color = (entrada[0] - 48);
                    adder -> led = (entrada[2] - 48);
                    adder -> estado = ENVIANDO_MENSAJE_DE_HORA; //Si es valido se pregunta la hora del evento 
                }
            }
            break;
        case(ENVIANDO_MENSAJE_PARA_APAGAR_INSTRUCCIONES): //Estado para enviar informacion sobre si se requiere apagar
            if (enviarMensaje(FORMATO_DE_APAGAR)) {
                adder -> estado = ENVIANDO_MENSAJE_DE_PARAM;
            }
            break;
        case(RECIBIENDO_INFORMACION_DE_APAGAR)://Recibe la informacion
            if (buscarEntrada(entrada, 2)) {
                if ((entrada[0] < '8' && entrada[0] >= '0') && entrada[1] == ';') { //Si es valida
                    adder -> led = (entrada[0] - 48);
                    adder -> estado = ENVIANDO_MENSAJE_DE_HORA; //Cambia de estado
                }
            }
            break;
        case(ENVIANDO_MENSAJE_DE_HORA): //Se envia info sobre el formato de la hora
            if (enviarMensaje(FORMATO_DE_HORA)) {
                adder -> estado = RECIBIENDO_HORA;
            }
            break;
        case(RECIBIENDO_HORA):
            if (pedirHora(&(adder -> tiempo), manager)) { //Cuando se introduzca una hora valida
                adder -> estado = ENVIANDO_MENSAJE_DE_FECHA; //Se pide fecha
            }
            break;
        case(ENVIANDO_MENSAJE_DE_FECHA): //Informacion sobre como enviar una fecha
            if (enviarMensaje(FORMATO_DE_FECHA)) {
                adder -> estado = RECIBIENDO_FECHA;
            }
            break;
        case(RECIBIENDO_FECHA):
            if (pedirFecha(&(adder -> tiempo), manager)) {
                adder -> estado = CREANDO_EVENTO; //Si la fecha es valida se procede a crear el evento
            }
            break;
        case(CREANDO_EVENTO):
        {
            //Se crea el evento y se le colocan los atributos marcados por el usuario
            app_event_t evento;
            uint32_t tiempoFinal;
            tiempoFinal = mktime(&(adder -> tiempo));
            evento.color = adder -> color;
            evento.command = adder -> command;
            evento.param = adder -> led;
            evento.time = tiempoFinal; //Con este tiempo sabremos cuando activar el evento

            eventos[evento.param] = evento; //Se pone el evento en el array en la posicion correspondiente al numero de led
            return true; //operacion exitosa
        }

    }
    return false;
}

void inicilizarEventos() {
    int i;
    for (i = 0; i < EVENTOS_MAXIMOS; i++) {
        eventos[i].command = 0xFF; //Se ponen todos los eventos a no configurado. Solo se usa al principio
    }
}

bool quitarEvento(event_kicker_t *kicker, uint8_t *entrada) {
    switch (kicker -> estado) {
        case(ENVIANDO_INSTRUCCIONES): //Envia instrucciones al usuario
            if (enviarMensaje(FORMATO_QUITAR_EVENTO)) {
                kicker -> estado = RECIBIENDO_ENTRADA;
            }
            break;
        case(RECIBIENDO_ENTRADA):
            if (buscarEntrada(entrada, 2)) { //La entrada es el led que se prendio, que es igual al indice en el array
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
                eventoABorrar->command = 0xFF; //Se pone el evento a borrar como no configurado
            }
            kicker -> estado = COMUNICANDO;
            break;
        }
        case (COMUNICANDO):
            enviarMensaje(ELEMENTO_BORRADO); //Se notifica el exito al usuario
            return true;
    }
    return false;
}

bool consultarListaDeEventos(event_voicer_t * voicer, char * salida) {
    switch (voicer -> estado) {
        case(SUMAR_CONTADOR): //Suma el contador que itera sobre el array de eventos
            if (voicer -> contador >= EVENTOS_MAXIMOS) { //Si llego al final del array 
                return true; //Notifica operacion exitosa
            } else {
                (voicer -> contador)++;
                if (eventos[(voicer -> contador)].command == 0xFF) { //Suma hasta que encuentra una tarea valida
                    voicer -> estado = SUMAR_CONTADOR; //Si la tarea no es valida sigue sumando
                } else {
                    voicer -> estado = ENVIANDO_POSICION;//Si es valida comunica su informacion
                }
            }
            return false;
        case(ENVIANDO_FORMATO): //Notifica al usuario el formato del output
            if (enviarMensaje(FORMATO_DE_AGENDA)) {
                if (eventos[voicer -> contador].command == 0xFF) { //Si la primera actividad no es valida
                    voicer -> estado = SUMAR_CONTADOR; //Suma el iterador
                } else {
                    voicer -> estado = ENVIANDO_POSICION; //Sino comunica la informacion
                }
            }
            return false;
        case(ENVIANDO_POSICION): //Envia la poscicion en el array
            salida[0] = voicer -> contador + 48;
            salida[1] = ',';
            salida[2] = ' ';
            if (enviarMensaje(salida)) {
                voicer -> estado = ENVIANDO_COLOR;
            }
            return false;
        case(ENVIANDO_COLOR): //Envia el color 
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
        case(ENVIANDO_TIEMPO): //Envia la hora en el cual se ejecutara
        {
            struct tm *tiempoFormateado;
            
            char tiempo[7];
            time_t tiempoPlano;
            
            tiempoPlano = eventos[voicer -> contador].time;
            tiempoFormateado = gmtime(&tiempoPlano);
            
            strftime(tiempo, 7, "%H:%M\r\n",tiempoFormateado);
            if (enviarMensaje(tiempo)) {
                voicer -> estado = SUMAR_CONTADOR; //Si termino va a sumar contador
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
