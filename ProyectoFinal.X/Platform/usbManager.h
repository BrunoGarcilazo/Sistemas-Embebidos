/* ************************************************************************** */
/** En este archivo estan algunas funciones para el manejo de las entradas
 * y salidas del usb.

  @Company
    UCUDAL

  @File Name
    usbManager.h

  @Summary
    Manejo del IO del USB

  @Description
    Implementar funciones que permitar el envio y recibimiento de packetes via USB
 */
/* ************************************************************************** */

#ifndef _USB_MANAGER_H    /* Guard against multiple inclusion */
#define _USB_MANAGER_H


/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
#include <stdint.h>
#include <stdbool.h>

/* ************************************************************************** */
/* Section: Funciones de interfaz                                             */
/* ************************************************************************** */

/**
 * Termina cuando se ha recibido algo.
 * Lo coloca en el array de entrada.
 * Es una funcion bloqueante
 * @param buffer Buffer al cual se carga el input
 * @param large Largo del  input requerido
 */
void buscarEntrada(uint8_t * buffer, uint8_t large);


/**
 * Este metodo envia un mensaje una vez por el usb
 * Es bloqueante, hasta que no se envie no termina
 * @param mensaje Array de chars a enviar
 */
void enviarMensaje(char *mensaje);

#endif /* _USB_MANAGER_H */
/* *****************************************************************************
 End of File
 */
