/** Menu

  @Company
    UCUDAL

  @File Name
    Menu.h

  @Summary
    Contiene la tarea menu y las variables asociadas

  @Description
    La tarea menu muestra el menu y espera por un input.
    Cuando el input es valido se crea una funcion que responde al comando.
 */

#ifndef _MENU_H
#define _MENU_H

// <editor-fold defaultstate="collapsed" desc="Tipos de datos">
/*Representa el estado del menu*/
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
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Fuciones de interfaz">
/**
 * Esta tarea se encarga de enviar el menu, esperar una respuesta y llamar a la funcion correspondiente
 * para responder al usuario
 * @param p_params
 */
void menu();
// </editor-fold>

#endif
