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

/*Menu*/
#define MENU "\r\nBienvenido\r\nMenu\r\nPara seleccionar una opcion envie la tecla presente a la izquierda de la funcion\r\nNota: 'a','c','d' y 'e' son criticas para el funcionamiento del sistema\r\na - Fijar hora del reloj RTC\r\nb - Consultar hora del RTC\r\nc - Agregar evento\r\nd - Quitar evento\r\ne - Consultar lista de eventos\r\n\0"


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */

/* ************************************************************************** */

typedef enum {
    /**
     * Refiere al estado normal donde el usuario debe pedir algo.
     */
    EN_MENU,

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

/**
 * Esta tarea se encarga de enviar el menu, esperar una respuesta y llamar a la funcion correspondiente
 * para responder al usuario
 * @param p_params
 */
void menu();

#endif /* _MENU_H */
