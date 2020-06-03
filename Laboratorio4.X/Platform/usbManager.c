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
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "../mcc_generated_files/usb/usb_device_cdc.h" 


void buscarEntrada(uint8_t * buffer, uint8_t large) {
    int8_t numBytes;
    while (true) {
        CDCTxService();
        if ((USBGetDeviceState() < CONFIGURED_STATE) ||
                (USBIsDeviceSuspended() == true)) {
            //Either the device is not configured or we are suspended
            //so we don't want to do execute any application code
            //go back to the top of the while loop
        } else {
            if (USBUSARTIsTxTrfReady()) {
                numBytes = getsUSBUSART(buffer, large);
                if (numBytes > 0) {
                    return;
                }
            }
        }
    }
}

void enviarMensaje(char *mensaje) {
    bool enviado = false;
    while (!enviado) {
        CDCTxService();
        if ((USBGetDeviceState() < CONFIGURED_STATE) ||
                (USBIsDeviceSuspended() == true)) {
        } else {
            if (USBUSARTIsTxTrfReady()) {
                putsUSBUSART(mensaje); //Si pudo enviar el mensaje devuelve 
                enviado = true;
            }
        }
    }

}


/* *****************************************************************************
 End of File
 */