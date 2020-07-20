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
#include "semphr.h"
#include  <stdint.h>
#include  <stdbool.h>
#include "../System/conversiones.h"
#include "interfazConversiones.h"


SemaphoreHandle_t medir;

void interfazTermometro(void* params) {
    ultimaMedida = 0;
    TaskHandle_t conversionHandler = NULL;
    medir = xSemaphoreCreateBinary();
    xTaskCreate(conversiones, "Conversiones", configMINIMAL_STACK_SIZE + 500, NULL, tskIDLE_PRIORITY + 2, &conversionHandler);
    while (1) {
        if (boton2Flag) {
            if (xSemaphoreTake(medir,0) == pdTRUE) { //Si el semaforo esta libre
                xSemaphoreTake(medir,0); //Detengo la medida
            } else { //Si el semaforo no esta libre 
                xSemaphoreGive(medir); //Permito medir
            }
            boton2Flag = false;
        }
    }
}
