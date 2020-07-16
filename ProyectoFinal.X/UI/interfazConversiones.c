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

#include  "FreeRTOS.h"
#include  "task.h"
#include  <stdint.h>
#include  <stdbool.h>
#include "../System/conversiones.h"
#include "interfazConversiones.h"

void interfazTermometro(void* params) {
    ultimaMedida = 0;
    TaskHandle_t conversionHandler = NULL;
    while (1) {
        if (boton2Flag) {
            if (dispositivo.midiendo) {
                vTaskDelete(conversionHandler);
                dispositivo.midiendo = false;
            } else {
                xTaskCreate(conversiones, "Conversiones", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 5, &conversionHandler);
            }
            boton2Flag = false;
        } else {
            vTaskDelay(pdMS_TO_TICKS(250));
        }
    }
}
