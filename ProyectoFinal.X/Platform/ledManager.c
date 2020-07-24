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

// <editor-fold defaultstate="collapsed" desc="Includes">
/*FreeRtos*/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/*MCC*/
#include "../mcc_generated_files/pin_manager.h"

/*Archivos del sistema*/
#include "WS2812.h"
#include "ledManager.h"
#include "../System/conversiones.h"
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Funciones de Interfaz">
/**
 * Pone todos los leds en negro
 */
void apagarLeds() {
    uint8_t j;
    for (j = 0; j < MAX_LEDS; j++) {
        coloresLeds[j] = BLACK;
    }
    WS2812_send(coloresLeds, MAX_LEDS);
}

/** Metodo llamado cuando se realiza una Medicion
 * Si los Leds estaban en Negro, se cambian a Azul y viceversa.
 */
void invertirLedsMedicion() {
    uint8_t i;
    if (coloresLeds[0].b == 0) {
        for (i = 0; i < MAX_LEDS; i++) {
            coloresLeds[i] = BLUE;
        }
    } else {
        apagarLeds();
    }
    WS2812_send(coloresLeds, MAX_LEDS);
}

/**
 * Prende los leds RGB en rojo, espera dos segundos y los apaga.
 * Se activa mediante un semaforo
 * @param p_params
 */
void prenderLedsRojosPor2Seg(void *p_params) {
    uint8_t i;
    while (1) {
        if (xSemaphoreTake(prenderYapagarLucesRojas,0) == pdTRUE) {
            for (i = 0; i < MAX_LEDS; i++) {
                coloresLeds[i] = RED;
            }
            WS2812_send(coloresLeds, MAX_LEDS);
            vTaskDelay(pdMS_TO_TICKS(2000));
            apagarLeds();
        } else {
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}

/**
 * Prende los leds RGB en verde, espera dos segundos y los apaga.
 * Se activa mediante un semaforo
 * @param p_params
 */
void prenderLedsVerdesPor2Seg(void *p_params) {
    uint8_t i;
    while (1) {
        if (xSemaphoreTake(prenderYapagarLucesVerdes,0) == pdTRUE) {
            for (i = 0; i < MAX_LEDS; i++) {
                coloresLeds[i] = GREEN;
            }
            WS2812_send(coloresLeds, MAX_LEDS);
            vTaskDelay(pdMS_TO_TICKS(2000));
            apagarLeds();
        } else {
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}


// </editor-fold>
