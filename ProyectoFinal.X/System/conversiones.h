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
#include <time.h>
#include "../Communications/GPS.h"

typedef struct{
    float temperaturaRegistrada;
    time_t tiempo;
    GPSPosition_t posicion;
} medida_t ;

medida_t mediciones[200];
uint8_t ultimaMedida;

void alertarPersona(void *p_params);
void conversiones(void *p_params);

#endif
