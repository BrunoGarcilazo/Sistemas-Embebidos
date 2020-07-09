/* ************************************************************************** */
/** Conversiones

  @Company
    UCUDAL

  @File Name
    conversiones.h

  @Summary
    Se encarga de las mediciones de temperatura del dispositivo

  @Description
    Mide, promedia, convierte la medida a grados. Prende leds cuando esta midiendo y luego leds de colores 
    segun la salud del paciente.
 */
/* ************************************************************************** */

#ifndef _CONVERSIONES_H    /* Guard against multiple inclusion */
#define _CONVERSIONES_H




/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include <stdint.h>

typedef struct{
    uint8_t IdDelRegistro;
    uint8_t temperaturaRegistrada;
} medida_t ;

medida_t mediciones[200];
uint8_t ultimaMedida;

uint16_t samplesConversiones[10];

void conversiones(void *p_params);

#endif

/* *****************************************************************************
 End of File
 */
