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

bool buscarEntrada(uint8_t * buffer, uint8_t large) {
    int8_t numBytes;
    CDCTxService();
    if ((USBGetDeviceState() < CONFIGURED_STATE) ||
            (USBIsDeviceSuspended() == true)) {
        //Either the device is not configured or we are suspended
        //so we don't want to do execute any application code
        //go back to the top of the while loop
        return false;
    } else {
        if (USBUSARTIsTxTrfReady()) {
            numBytes = getsUSBUSART(buffer, large); //El mensaje se guardara en buffer
            if (numBytes > 0) {
                return true; //Si llego un mensaje devuelve true
            }
        }
    }
    return false;
}

bool enviarMensaje(char *mensaje) {
    CDCTxService();
    if ((USBGetDeviceState() < CONFIGURED_STATE) ||
            (USBIsDeviceSuspended() == true)) {
        return false;
    } else {
        if (USBUSARTIsTxTrfReady()) {
            putsUSBUSART(mensaje); //Si pudo enviar el mensaje devuelve 
            return true;
        }
    }
    return false;
}



/* *****************************************************************************
 End of File
 */
