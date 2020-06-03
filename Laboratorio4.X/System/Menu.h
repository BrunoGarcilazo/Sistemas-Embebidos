/* ************************************************************************** */
/** Menu

  @Company
    UCUDAL

  @File Name
    Menu.h

  @Summary
    Contiene la tarea menu y las variables asociadas

  @Description
    La tarea menu muestra el menu y espera por un input.
    Cuando el input es valido se crea una funcion.
 */
/* ************************************************************************** */

#ifndef _MENU_H    /* Guard against multiple inclusion */
#define _MENU_H

//Formato de inputs
#define FORMATO_DE_HORA "\r\nIngrese hora en formato hh:mm:ss\r\n"
#define FORMATO_DE_FECHA "\r\nIngrese fecha en formato dd/mm/aaaa\r\n"
#define FORMATO_DE_APAGAR "\r\nIngrese la informacion en el siguiente formato\r\nNumeroDeLED;\r\n"
#define FORMATO_PARA_PRENDER "\r\nIngrese la informacion en el siguiente formato\r\nColor,NumeroDeLED\r\n"
#define FORMATO_DE_AGENDA "\r\nCada linea representa un evento y tiene el siguiente formato:\r\n posicion, color, hora de ejecuccion\r\n"
#define FORMATO_QUITAR_EVENTO "\n\rIngrese la posicion del evento que desea quitar con este formato\n\rPoscion.\r\nLas posiciones van de 0 a 7\r\n"



#define SELECCION_DE_LED "\r\n¿Que Led del 0 al 7 desea accionar?\r\n"
#define PREGUNTA_POR_COLOR_DE_LED "\r\n¿De que color desea encender el Led?\r\n0- Blanco\r\n1- Rojo\r\n2- Azul\r\n3- Verde\r\n"
#define PREGUNTA_DE_COMAND "\r\nIngrese 1 si desea prender 0 si desea apagar un LED\r\n"
#define ELEMENTO_BORRADO "\r\nElemento Borrado Satisfactoriamente\r\n"
#define MENU "\r\nBienvenido\r\nMenu\r\nPara seleccionar una opcion envie la tecla presente a la izquierda de la funcion\r\na - Fijar hora del reloj RTC\r\nb - Consultar hora del RTC\r\nc - Agregar evento\r\nd - Quitar evento\r\ne - Consultar lista de eventos\r\n"


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

typedef enum {
    /**
     * Indica si la fecha y hora fueron inicializadas
     */
    NO_INICIALIZADA,

    /**
     * Refiere al estado normal donde el usuario debe pedir algo.
     */
    EN_MENU,

    /**
     * 
     */
    EN_MOSTRAR_HORA,

    /**
     * Cuando se esta requiriendo al usuario que ingrese un tiempo
     */
    EN_INGRESAR_HORA,

    /**
     * Cuando el usuario esta agregando un evento
     */
    EN_AGREGAR_EVENTO,

    /**
     * Cuando el usuario este quitando un evento
     */
    EN_QUITAR_EVENTO,

    /**
     * Cuando el usuario este consultando la lista de eventos
     */
    EN_CONSULTAR_LISTA_DE_EVENTOS,

    /**
     * Cuando el sistema no ha recibido nada
     */
    EN_ESPERA,

} MENU_STATUS;

/* ***************************************************************************** */
/* ***************************************************************************** */
/* Funciones de interfaz                                                         */
/* ***************************************************************************** */
/* ***************************************************************************** */

void menu(void *p_params);

#endif /* _MENU_H */
