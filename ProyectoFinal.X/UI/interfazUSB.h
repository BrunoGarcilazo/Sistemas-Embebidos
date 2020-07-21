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
#include "semphr.h"


/*Constantes para pedir datos*/
#define PEDIDO_DE_ID "\r\nIngrese ID unico del Dispositivo. Numero entero de 32 bits\r\n"
#define PEDIDO_DE_UMBRAL "\r\nIngrese umbral de temperatura. Entre 32.0 C y 42.0C\r\n"
#define FORMATO_DE_TELEFONO "\r\nIngrese numero de telefono celular. Formato: 096123456\r\n"

#define DATO_CORRECTO "\r\nDato valido. Muchas Gracias\r\n"

#define ASCII_TO_INT_DIFFERENCE 48

extern SemaphoreHandle_t inicializado;

void interfazUSB(void* params);

void mantenimientoUSB(void * p_param);

void inicalizarUSB(void * p_param);

bool enUSB;

#endif /* _INTERFAZ_USB_H */

/* *****************************************************************************
 End of File
 */
