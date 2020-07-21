/* ************************************************************************** */
/** En este archivo hay funciones que manejan el RTC en base a interaccion via USB

  @Company
    UCUDAL

  @File Name
    rtcManager.c

  @Summary
    Funciones que usan el RTC y USB

  @Description
    Procesar input del usuario e interactuar con el USB
 */
/* ************************************************************************** */
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "FreeRTOS.h"
#include "../freeRTOS/include/semphr.h"
#include "task.h"


/*Libraries*/
#include <time.h>
#include <stdbool.h>
#include <string.h>

/*MCC Includes*/
#include "../mcc_generated_files/rtcc.h"
#include "../mcc_generated_files/pin_manager.h"

#include "gpsManager.h"
#include "../UI/interfazUSB.h"
#include "../UI/interfazConversiones.h"
#include "../System/menu.h"

SemaphoreHandle_t tramaValida;

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/**
 * El sistema trata de obtener una trama valida cada 100ms. Una vez conseguida la coloca en el RTC.
 * Una vez colocada la hora en el RTC el sistema libera un semaforo para que se pueda usar la hora y la actualiza
 * cada un segundo.
 * @param p_params
 */
void mantenerGPS(void *p_params) {
    tramaValida = xSemaphoreCreateBinary();

    while (1) {
        //SIM808_getNMEA(dispositivo.trama);
        while (!SIM808_validateNMEAFrame(dispositivo.trama)) {
            xSemaphoreTake(tramaValida, 0);
            SIM808_getNMEA(dispositivo.trama);
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        GPS_getUTC(&tiempoDelSistema, dispositivo.trama);
        RTCC_TimeSet(&tiempoDelSistema);
        xSemaphoreGive(tramaValida);

        LEDA_Toggle();
        vTaskDelay(pdMS_TO_TICKS(1000)); //Si la trama es valida y la hora esta seteada que espere segundo
    }
}
/* *****************************************************************************
 End of File
 */