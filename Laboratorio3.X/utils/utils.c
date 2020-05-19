
#include "../utilss/utils.h"
#include <stdint.h>
#include <stdbool.h>
#include "../mcc_generated_files/system.h"
#include "../mcc_generated_files/pin_manager.h"
#include "../mcc_generated_files/usb/usb_device_cdc.h" 

bool UT_delayms(ut_tmrDelay_t* p_timer, uint32_t p_ms) {
    /*
     * El delay es una maquina de estados que se representa con un switch
     * */

    switch (p_timer->state) {
        case UT_TMR_DELAY_INIT:
            p_timer->startValue = TMR2_SoftwareCounterGet();
            p_timer->state = UT_TMR_DELAY_WAIT;
            break;

        case UT_TMR_DELAY_WAIT:
            if ((TMR2_SoftwareCounterGet() - (p_timer->startValue)) >= p_ms) {
                p_timer->state = UT_TMR_DELAY_INIT;
                return true;
            }
            break;

        default:
            break;
    }
    return false;

}
void iniciarMenu() {
        bool primeraVez = true;
        char bienvenida[] = "hola" ;
        while (primeraVez) {
            CDCTxService();
            if ((USBGetDeviceState() < CONFIGURED_STATE) ||
                    (USBIsDeviceSuspended() == true)) {
                //Either the device is not configured or we are suspended
                //  so we don't want to do execute any application code
                continue; //go back to the top of the while loop
            } else {
                if (USBUSARTIsTxTrfReady()) {
                    putsUSBUSART(bienvenida);
                }
            }
        }

    }

