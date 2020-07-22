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
        
        
// <editor-fold defaultstate="collapsed" desc="Include Files">

/*Archivos del sistema*/
#include "WS2812.h"

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Constants and Macros">
#define MAX_LEDS 8 //Numero de leds de la placa (constante para crear array)
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Tareas">
/**
 * Prende los leds RGB en rojo, espera dos segundos y los apaga.
 * Se activa mediante un semaforo
 * @param p_params
 */
void prenderLedsRojosPor2Seg(void *p_params);

/**
 * Prende los leds RGB en Verde, espera dos segundos y los apaga.
 * Se activa mediante un semaforo
 * @param p_params
 */
void prenderLedsVerdesPor2Seg(void *p_params);
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Funciones de interfaz">
/**
 * Si los leds RGB estaban en negro los pone en azul.
 * Si estaban en azul los pone en negro
 */
void invertirLedsMedicion();

/**
 * Pone todos los leds en negro
 */
void apagarLeds();
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Global Data">
ws2812_t coloresLeds[MAX_LEDS];
// </editor-fold>



#endif
