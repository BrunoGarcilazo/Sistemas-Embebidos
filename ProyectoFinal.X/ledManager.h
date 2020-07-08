/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _LED_MANAGER_H    /* Guard against multiple inclusion */
#define _LED_MANAGER_H
        
        
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "Platform/WS2812.h"

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */

void invertirLedsMedicion();

ws2812_t coloresLeds[8];


#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
