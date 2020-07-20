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
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include "WS2812.h"
#include "ledManager.h"
#include "FreeRTOS.h"
#include "task.h"
#include "../mcc_generated_files/pin_manager.h"
#include "../System/conversiones.h"
#include "semphr.h"

/** Metodo llamado cuando se realiza una Medicion
 * Si los Leds estaban en Negro, se cambian a Azul y viceversa.
 */
void invertirLedsMedicion() {
    uint8_t i;
    if (coloresLeds[0].b == 0) {
        for (i = 0; i < MAX_LEDS; i++) { //PREGUNTAR PORQUE NO ANDA SIZEOF
            coloresLeds[i] = BLUE;
        }
    } else {
        for (i = 0; i < MAX_LEDS; i++) {
            coloresLeds[i] = BLACK;
        }
    }
    WS2812_send(coloresLeds, MAX_LEDS);
}

void prenderLedsRojosPor2Seg(void *p_params) {
    uint8_t i, j;
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

void prenderLedsVerdesPor2Seg(void *p_params) {
    uint8_t i, j;
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

void apagarLeds() {
    uint8_t j;
    for (j = 0; j < MAX_LEDS; j++) {
        coloresLeds[j] = BLACK;
    }
    WS2812_send(coloresLeds, MAX_LEDS);
}


/* *****************************************************************************
 End of File
 */
