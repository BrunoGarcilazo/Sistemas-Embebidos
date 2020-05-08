
#include "utils.h"
#include "stdint.h"
#include "stdbool.h"

extern int global_data;
extern bool aux;
int interrupciones;

typedef struct
{
uint32_t startValue;
UT_TMR_DELAY_STATE state;
} ut_tmrDelay_t;

typedef enum
{
UT_TMR_DELAY_INIT,
UT_TMR_DELAY_WAIT
} UT_TMR_DELAY_STATE;




bool UT_delayms(ut_tmrDelay_t* p_timer, uint32_t p_ms){
    if(!aux){
        p_timer->startValue = p_ms;
        aux = true;
    }
    
    if(ut_tmrDelay_t.startValue > 0){
        TMR2_Start(); 
        return false;
    }else{
        TMR2_Stop();
        return true;
    }
    
}

void DecrementarContador(){
    interrupciones = interrupciones - 1;
}
