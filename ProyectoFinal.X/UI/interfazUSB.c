/* ************************************************************************** */

#include "interfazUSB.h"

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
#include  "../mcc_generated_files/usb/usb_device_cdc.h"
#include  "FreeRTOS.h"
#include  "task.h"
#include  <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../conversiones.h"

void interfazUSB(void* params) {
    uint8_t buffer[4];
    uint16_t numBytes;
    TaskHandle_t conversionHandler = NULL;

    while (1) {
        /*if (USBUSARTIsTxTrfReady()) {
            numBytes = getsUSBUSART(buffer, sizeof (buffer));
            if (numBytes > 0) {
                if (!dispositivo.inicializado) {
                    inicializar();
                }
                menu();
            }
        }*/
        if (boton2Flag){
            if (dispositivo.midiendo){
                vTaskDelete(conversionHandler);
                dispositivo.midiendo = false;
            } else {
                xTaskCreate(conversiones, "Conversiones", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, &conversionHandler);
                //dispositivo.midiendo = false;
            }
            boton2Flag = false;
        } else {
            vTaskDelay(pdMS_TO_TICKS(250));
        }
    }
}

bool pedirID() { // sin terminar
    uint8_t entrada[32]; // Array donde se va a recibir la entrada
    memset(entrada, 0, sizeof (entrada)); //Se limpia la entrada
    buscarEntrada(entrada, sizeof (entrada));
    uint32_t k;
    int i;
    for (i = 0; i < 32; i++) { // Metodo de conversion de Array de char a int : shorturl.at/acoAL
        k = 10 * k + (entrada[i] - ASCII_TO_INT_DIFFERENCE);
    }
    dispositivo.dispositivoID = k;
}

bool pedirNumeroDeContacto() { // sin terminar
    uint8_t entrada[8];

    memset(entrada, 0, sizeof (entrada)); //Se limpia la entrada
    buscarEntrada(entrada, sizeof (entrada));

    uint8_t i, n;
    uint8_t k;
    for (i = 0; i < n; i++) {
        k = 10 * k + (entrada[i] - ASCII_TO_INT_DIFFERENCE);
    }

    dispositivo.numeroDeContacto = k;
}

bool pedirTemperatura() {
    uint8_t entrada[2];
    uint8_t temperatura;
    buscarEntrada(entrada, sizeof (entrada));
    temperatura = ((10 * entrada[0] - ASCII_TO_INT_DIFFERENCE) + (entrada[1] - ASCII_TO_INT_DIFFERENCE));
    dispositivo.umbralDeTemperatura = temperatura;
}


/* *****************************************************************************
 End of File
 */
