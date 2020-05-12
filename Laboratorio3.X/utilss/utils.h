#ifndef _EXAMPLE_FILE_NAME_H    /* Guard against multiple inclusion */
#define _EXAMPLE_FILE_NAME_H

#define UT_DELAY_CYCLES 50000
#include "stdint.h"
#include "stdbool.h"


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#define EXAMPLE_CONSTANT 0

    typedef enum {
        UT_TMR_DELAY_INIT,
        UT_TMR_DELAY_WAIT
    } UT_TMR_DELAY_STATE;

    typedef struct {
        uint32_t startValue;
        UT_TMR_DELAY_STATE state;
    } ut_tmrDelay_t;

    int ExampleFunction(int param1, int param2);
    /*
     * Esta funcion decrementa un contador de interrupciones del timer dos.
     * De esta forma contamos milisegundos.
     */
    void DecrementarContador(void);

    bool UT_delayms(ut_tmrDelay_t* p_timer, uint32_t p_ms);




    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
