/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include "Platform/WS2812.h"
#include "../ProyectoFinal.X/ledManager.h"
#include "FreeRTOS.h"
#include "task.h"
#include "mcc_generated_files/pin_manager.h"


/** Metodo llamado cuando se realiza una Medicion
 * Si los Leds estaban en Negro, se cambian a Azul y viceversa.
 */
void invertirLedsMedicion() {
    uint8_t i;
    if (coloresLeds[0].b == 0) {
        for (i = 0; i < 8; i++) { //PREGUNTAR PORQUE NO ANDA SIZEOF
            coloresLeds[i] = BLUE;
        }
    } else {
        for (i = 0; i < 8; i++) {
            coloresLeds[i] = BLACK;
        }

    }
    WS2812_send(coloresLeds, 8);
}

void prenderLedsRojosPor2Seg(void *p_params) {
    uint8_t i,j;
    for (i = 0; i < 8; i++) {
        coloresLeds[i] = RED;
    }
    WS2812_send(coloresLeds, 8);
    
    vTaskDelete(NULL);
}

void prenderLedsVerdesPor2Seg(void *p_params) {
    uint8_t i;
    for (i = 0; i < 8; i++) {
        coloresLeds[i] = GREEN;
    }
    WS2812_send(coloresLeds, 8);
//    vTaskDelay(pdMS_TO_TICKS(2000));
//    for (i = 0; i < 8; i++) {
//        coloresLeds[i] = BLACK;
//    }
//    WS2812_send(coloresLeds, 8);   
    vTaskDelete(NULL);
}

void apagarLeds(void *p_params){
    
    uint8_t j;
    for (j = 0; j < 8; j++) {
        coloresLeds[j] = BLACK;
    }
    WS2812_send(coloresLeds, 8);
}


/* *****************************************************************************
 End of File
 */
