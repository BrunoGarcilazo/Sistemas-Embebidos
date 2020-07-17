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

#ifndef _LED_MANAGER_H
#define _LED_MANAGER_H
        
        
/* Section: Included Files                                                    */

#include "WS2812.h"

#define MAX_LEDS 8

void prenderLedsRojosPor2Seg(void *p_params);

void prenderLedsVerdesPor2Seg(void *p_params);

void invertirLedsMedicion();

void apagarLeds();

ws2812_t coloresLeds[MAX_LEDS];


#endif
