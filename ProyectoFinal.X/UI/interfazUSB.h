/** InterfazUSB

  @Company
    UCUDAL

  @File Name
    interfazUSB.c

  @Summary
    Gestion de la interaccion via USB de la placa

  @Description
    Tareas y funciones que se encargan del ingreso y salida de datos via USB
*/

#ifndef _interfazUSB_H
#define _interfazUSB_H

// <editor-fold defaultstate="collapsed" desc="Includes">
//FreeRtos
#include "semphr.h"

//Librerias
#include <stdbool.h>
#include <stdint.h>
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Constantes y Macros">
//Pedidos de datos
#define PEDIDO_DE_ID "\r\nIngrese ID unico del Dispositivo. Numero entero de 32 bits\r\n"
#define PEDIDO_DE_UMBRAL "\r\nIngrese umbral de temperatura. Entre 32.0 C y 42.0C\r\n"
#define FORMATO_DE_TELEFONO "\r\nIngrese numero de telefono celular. Formato: 09XXXXXXX\r\n"

//Dato correcto
#define DATO_CORRECTO "\r\nDato valido. Muchas Gracias\r\n"

//Diferencia para convertir un caracter a su valor numerico
#define ASCII_TO_INT_DIFFERENCE 48
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Variables">
/**
 * Este semaforo hace que no se ejecute ninguna actividad hasta que se ingresen ID, numero de contacto y umbral de
 * temperatura. Tambien se usa para evitar que se ejecute una medicion si el menu usb esta siendo ejecutado y vice
 * versa
 */
extern SemaphoreHandle_t inicializado;

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Tareas">
/**
 * Esta tarea se encarga de reaccionar ante la necesidad de ejecutar el menu USB o incializar el dispositivo.
 * Antes de ejecutar el menu verifica si no se esta realizando una medida de temperatura.
 * @param params
 */
void interfazUSB(void* params);

/**
 * Tarea que se encarga de ejecutar CDCTxService() cada 10 ms cuando sea necesario.
 * @param p_param
 */
void mantenimientoUSB(void * p_param);
// </editor-fold>

#endif