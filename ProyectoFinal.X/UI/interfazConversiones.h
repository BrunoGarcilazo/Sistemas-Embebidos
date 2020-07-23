/* ************************************************************************** */
/** Interfaz Conversiones

  @Company
    UCUDAL

  @File Name
    interfazConversiones.h

  @Summary
    Tareas que se encargan de lo relacionado con las medidas de temperatura del sistema

  @Description
    Controlar, guardar y tomar las conversiones analogico a digital del sistema
 */
/* ************************************************************************** */

#ifndef _INTERFAZ_CONVERSIONES_H    /* Guard against multiple inclusion */
#define _INTERFAZ_CONVERSIONES_H


// <editor-fold defaultstate="collapsed" desc="Incluidos">
//FreeRtos
#include "FreeRTOS.h"
#include "semphr.h"
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Tipos de Datos">
typedef struct {
    uint32_t dispositivoID; // ID unico del dispositivo
    uint8_t numeroDeContacto[15]; // Numero de contacto de forma 96123456
    uint8_t trama[70]; //Trama de GPS del dispositivo
    float umbralDeTemperatura; // Umbral de Temperatura. 32 < umbral < 42
    bool inicializado;  /*Variable encargada de señalar si ID, Numero de contacto y umbral de temperatura fueron
                         * seteados via USB*/
} termometro_t;
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Variables">
/*
 *termometro_t encargado de modelar el estado del dispositivo.
 */
termometro_t dispositivo;

/**
 * Semaforo que señaliza si la tarea de conversiones puede ejecutar una conversion
 * Se usa para parar las medidas, por lo que en una conversion se hace muchas veces take a este semaforo
 */
extern SemaphoreHandle_t medir;

//Flag que indica el estado del boton2, se pone en true mediante una interrupcion
bool boton2Flag;
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Tareas">
/**
 * Tarea que gestiona la tarea de conversiones.
 * Chequea el valor de boton2Flag y comienza o termina una medida.
 * @param params
 */
void interfazTermometro(void* params);
// </editor-fold>

#endif
