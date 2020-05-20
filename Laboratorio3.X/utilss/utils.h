#ifndef _UTILS_H    /* Guard against multiple inclusion */
#define _UTILS_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */



/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */
#include <stdint.h>
#include <stdbool.h>


    typedef enum {
        UT_TMR_DELAY_INIT,
        UT_TMR_DELAY_WAIT
    } UT_TMR_DELAY_STATE;

    typedef struct {
        uint32_t startValue;
        UT_TMR_DELAY_STATE state;
    } ut_tmrDelay_t;

    bool UT_delayms(ut_tmrDelay_t* p_timer, uint32_t p_ms);

    void iniciarMenu();
    
    void cleanBuffer(uint8_t* buffer);

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
