/* ************************************************************************** */
/** Interfaz Conversiones

  @Company
    UCUDAL

  @File Name
    interfazConversiones.h

  @Summary
    Tareas que se encargan de lo relacionado con la gestion de medidas

  @Description
    Controlar cuando iniciar y terminar una conversion
 */

// <editor-fold defaultstate="collapsed" desc="Includes">
//FreeRtos
#include  "FreeRTOS.h"
#include  "task.h"
#include "semphr.h"

//Librerias
#include  <stdint.h>
#include  <stdbool.h>

//Archivos del programa
#include "../System/conversiones.h"
#include "interfazConversiones.h"
#include "interfazUSB.h"
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Variables">
SemaphoreHandle_t puedoMedir;
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Tareas">

void interfazTermometro(void* params) {
    ultimaMedida = 0; //Setea el contador para ingresar medidas a 0, de esta forma se empieza desde el principio
    puedoMedir = xSemaphoreCreateBinary(); //Crea el semaforo binario que controla las mediciones
    xTaskCreate(conversiones, "Conversiones", configMINIMAL_STACK_SIZE + 500, NULL, tskIDLE_PRIORITY + 2, NULL);
    while (1) {
        if (boton2Flag) { //Cuando el boton fue presionado
            if (xSemaphoreTake(inicializado, 0) == pdTRUE) {
                if (xSemaphoreTake(puedoMedir, 0) != pdTRUE) { //Si el semaforo no esta libre
                    xSemaphoreGive(puedoMedir); //Permito medir
                }
            } else {
                if (xSemaphoreTake(puedoMedir, 0) == pdTRUE) { //Si el semaforo esta libre
                    //Detengo la medida
                }
            }
            boton2Flag = false; //Ponemos el flag en false
        }
    }
}

// </editor-fold>

