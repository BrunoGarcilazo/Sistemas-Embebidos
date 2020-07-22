/** En este archivo estan algunas funciones para el manejo de las entradas
 * y salidas del usb.

  @Company
    UCUDAL

  @File Name
    usbManager.c

  @Summary
    Manejo del IO del USB

  @Description
    Implementar funciones que permitar el envio y recibimiento de packetes via USB
 */

// <editor-fold defaultstate="collapsed" desc="Interfaces">
/*Librerias*/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/*MCC*/
#include  "FreeRTOS.h"
#include  "task.h"
#include "../mcc_generated_files/usb/usb_device_cdc.h" 
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Funciones de Interfaz">
void buscarEntrada(uint8_t * buffer, uint8_t large) {
    int8_t numBytes; //Numero de bytes recibidos
    while (true) {
        if ((USBGetDeviceState() < CONFIGURED_STATE) ||
                (USBIsDeviceSuspended() == true)) {
        } else {
            if (USBUSARTIsTxTrfReady()) {
                numBytes = getsUSBUSART(buffer, large);
                if (numBytes > 0) { // Si se recibio una entrada
                    return; //Retorna
                }
            }
        }
    }
}

void enviarMensaje(uint8_t *mensaje) {
    bool enviado = false;
    do {
        if ((USBGetDeviceState() < CONFIGURED_STATE) ||
                (USBIsDeviceSuspended() == true)) {
        } else {
            if (USBUSARTIsTxTrfReady()) {
                putUSBUSART(mensaje,strlen(mensaje)); //Si pudo enviar el mensaje devuelve 
                enviado = true;
            }
        }
    } while (!enviado);
}
// </editor-fold>
