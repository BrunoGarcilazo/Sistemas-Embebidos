
#include "utils.h"
#include "stdint.h"
#include "stdbool.h"

int global_data;
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




bool UT_delayms(ut_tmrDelay_t* p_timer, uint32_t p_ms)
{
    
}

void DecrementarContador(){
    interrupciones = interrupciones - 1;
}
