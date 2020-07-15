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


/*Constantes para pedir datos*/
#define PEDIDO_DE_ID "Ingrese ID unico del Dispositivo. Numero entero de 32 bits\0"
#define PEDIDO_DE_UMBRAL "Ingrese umbral de temperatura. Entre 32C y 42C\0"
#define FORMATO_DE_TELEFONO "Ingrese numero de telefono celular. Formato: 096123456\0"

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
