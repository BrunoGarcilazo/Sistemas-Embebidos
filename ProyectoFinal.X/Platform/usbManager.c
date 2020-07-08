/* ************************************************************************** */
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
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/*Libraries*/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/*MCC Includes*/
#include "../mcc_generated_files/usb/usb_device_cdc.h" 

void buscarEntrada(uint8_t * buffer, uint8_t large) {
    int8_t numBytes; //Numero de bytes recibidos
    while (true) {
        CDCTxService();
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

void enviarMensaje(char *mensaje) {
    bool enviado = false;
    do {
        CDCTxService();
        if ((USBGetDeviceState() < CONFIGURED_STATE) ||
                (USBIsDeviceSuspended() == true)) {
        } else {
            if (USBUSARTIsTxTrfReady()) {
                putsUSBUSART(mensaje); //Si pudo enviar el mensaje devuelve 
                enviado = true;
            }
        }
    } while (!enviado);
}

void enviarMensaje(uint8_t mensaje) {
    bool enviado = false;
    do {
        CDCTxService();
        if ((USBGetDeviceState() < CONFIGURED_STATE) ||
                (USBIsDeviceSuspended() == true)) {
        } else {
            if (USBUSARTIsTxTrfReady()) {
                putUSBUSART(mensaje,mensaje.length); //Si pudo enviar el mensaje devuelve 
                enviado = true;
            }
        }
    } while (!enviado);
}
/* *****************************************************************************
 End of File
 */