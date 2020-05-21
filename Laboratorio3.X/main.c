
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
#include "mcc_generated_files/rtcc.h"
#include "utilss/scheludeManager.h" 
#include <time.h>

/*
                         Main application
 */

int main(void) {
    // initialize the device
    SYSTEM_Initialize();
    ut_tmrDelay_t timer;
    timer.state = UT_TMR_DELAY_INIT;
    schelude_t agenda;

    //Variables de luces
    bool luzPrendida = false;
    uint32_t ochociento = 800;
    uint32_t cuatrociento = 400;

    //Variables de USB

    uint8_t large = 10;
    uint8_t buffer[large];
    char menu[] = "\r\nBienvenido\r\nMenu\r\nPara seleccionar una opcion envie la tecla presente a la izquierda de la funcion\r\na - Fijar hora del reloj RTC\r\nb - Consultar hora del RTC\r\nc - Agregar evento\r\nd - Quitar evento\r\ne - Consultar lista de eventos\r\n";
    uint8_t numBytes;

    //Variables para input de usuario
    struct tm tiempo;

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
                    switch (buffer[0]) {
                        case('m'):
                            if (buffer[1] == 'e' & buffer[2] == 'n' & buffer[3] == 'u') {
                                putrsUSBUSART(menu);
                            }
                            break;
                        case('a'):
                            putrsUSBUSART("\r\nIngrese hora en formato horas-minutos-segundos. Sin caracteres en medio porfavor\r\n");
                            tiempo = pedirHora();
                            RTCC_TimeSet(&tiempo);
                            break;
                        case('b'):
                            putsUSBUSART("\r\nLa hora actual es:\r\n");
                            consultarHora();
                            break;
                        case ('c'):
                            putsUSBUSART("\r\nIngrese en este orden la informacion\r\nComando: 1 es prender, 0 es pagar");
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

