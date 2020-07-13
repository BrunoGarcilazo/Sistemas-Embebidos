/* ************************************************************************** */
/** LedManager

  @Company
    UCUDAL

  @File Name
    ledManager.c

  @Summary
    Metodos encargados de manejar los leds RGB

  @Description
    Prender y Apagar leds RGB. Seleccionando colores
 */
/* ************************************************************************** */

#ifndef _LED_MANAGER_H    /* Guard against multiple inclusion */
#define _LED_MANAGER_H
        
        
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "WS2812.h"

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */

void prenderLedsRojosPor2Seg(void *p_params);

void prenderLedsVerdesPor2Seg(void *p_params);

void invertirLedsMedicion();

void apagarLeds();

ws2812_t coloresLeds[8];


#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
