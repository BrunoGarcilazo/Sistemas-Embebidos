/* ************************************************************************** */
/**InterfazUSB

  @Company
    UCUDAL

  @File Name
    interfazUSB.c

  @Summary
    Gestion de la interaccion del mundo exterior con la placa 

  @Description
    Crear tareas para manejar la interaccion de la placa con el mundo
 */
/* ************************************************************************** */

#ifndef _interfazUSB_H    /* Guard against multiple inclusion */
#define _interfazUSB_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include <stdbool.h>
#include <stdint.h>

#define ASCII_TO_INT_DIFFERENCE 48

void interfazUSB(void* params);

void mantenimientoUSB(void * p_param);

void inicalizarUSB(void * p_param);

bool enUSB;

#endif /* _INTERFAZ_USB_H */

/* *****************************************************************************
 End of File
 */
