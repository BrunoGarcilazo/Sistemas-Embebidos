/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include <stdint.h>
#include <stdbool.h>
#include "../mcc_generated_files/usb/usb_device_cdc.h" 

bool buscarEntrada(uint8_t *  buffer, uint8_t large) {
    int8_t numBytes;
    while (1) {
        CDCTxService();
        if ((USBGetDeviceState() < CONFIGURED_STATE) ||
                (USBIsDeviceSuspended() == true)) {
            //Either the device is not configured or we are suspended
            //so we don't want to do execute any application code
            //go back to the top of the while loop
            continue;
        } else {
            if (USBUSARTIsTxTrfReady()) {
                numBytes = getsUSBUSART(buffer, large);
                if (numBytes > 0) {
                    return true;
                }
            }
        }
    }
}

/* *****************************************************************************
 End of File
 */
