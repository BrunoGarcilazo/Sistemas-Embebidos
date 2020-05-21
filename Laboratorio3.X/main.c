
/**
  Section: Included Files
 */

#include "utilss/utils.h"
#include "platform/WS2812.h"
#include "platform/rtcManager.h"
#include "platform/scheludeManager.h"
#include "platform/usbManager.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/usb/usb_device_cdc.h" 



/*
                         Main application
 */
int main(void) {
    // initialize the device
    SYSTEM_Initialize();
    ut_tmrDelay_t timer;
    timer.state = UT_TMR_DELAY_INIT;

    //Variables de luces
    bool luzPrendida = false;
    uint32_t ochociento = 800;
    uint32_t cuatrociento = 400;

    //Variables de USB
    int8_t numBytes;
    uint8_t buffer[10];

    
    while (1) {

        // Fijar la hora del reloj de tiempo real (RTC) del PIC.
        // Consultar la hora del RTC del PIC.
        // Agregar o quitar un evento de calendario.
        // Consultar la lista de eventos de calendario activos.

        CDCTxService();
        if ((USBGetDeviceState() < CONFIGURED_STATE) ||
                (USBIsDeviceSuspended() == true)) {
            //Either the device is not configured or we are suspended
            //so we don't want to do execute any application code
            //go back to the top of the while loop
            continue;
        } else {
            if (USBUSARTIsTxTrfReady()) {
                numBytes = getsUSBUSART(buffer, sizeof (buffer));
                if (numBytes > 0) {
                    switch (buffer[0]) {
                        case('m'):
                            if (buffer[1] == 'e' & buffer[2] == 'n' & buffer[3] == 'u') {
                                putrsUSBUSART("\r\nBienvenido\r\nMenu\r\nPara seleccionar una opcion envie la tecla presente a la izquierda de la funcion\r\n"
                                        "a - Fijar hora del reloj RTC\r\n"
                                        "b - Consultar hora del RTC\r\n"
                                        "c - Agregar evento\r\nd - Quitar evento\r\n"
                                        "e - Consultar lista de eventos\r\n");
                            }
                            break;
                        case('a'):
                            putrsUSBUSART("\r\nIngrese hora en formato horas-minutos-segundos. Sin caracteres en medio porfavor\r\n");
                            pedirHora();
                            break;
                        case('b'):
                            putsUSBUSART("\r\nLa hora actual es: \r\n");
                            consultarHora();
                            break;
                        case ('c'):
                            putsUSBUSART(buffer);
                            break;
                        case ('d'):
                            putsUSBUSART(buffer);
                            break;
                        default:
                            break;
                    }
                    cleanBuffer(buffer);
                }

            }
        }    

        switch (luzPrendida) {
            case false:
                if (UT_delayms(&timer, ochociento)) {
                    luzPrendida = true;
                    LEDB_SetHigh();
                }
                break;
            case true:
                if (UT_delayms(&timer, cuatrociento)) {
                    luzPrendida = false;
                    LEDB_SetLow();
                }
                break;
        }
    }
    return 1;
}