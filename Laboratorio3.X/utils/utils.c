
#include "../utilss/utils.h"
#include <stdint.h>
#include <stdbool.h>


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

