/** En este archivo hay funciones que se encargar de la gestion de la trama GPS

  @Company
    UCUDAL

  @File Name
    gpsManager.h

  @Summary
    Tareas que se encargan de manejar los datos obtenidos por el gps

  @Description
    Libera un semaforo cuando la trama es valida. Permite a otras tareas usar datos del GPS
 */

// <editor-fold defaultstate="collapsed" desc="Includes">
//Sistema Operativo
#include "FreeRTOS.h"
#include "../freeRTOS/include/semphr.h"
#include "task.h"

//Librerias
#include <time.h>
#include <stdbool.h>
#include <string.h>

//MCC
#include "../mcc_generated_files/rtcc.h"
#include "../mcc_generated_files/pin_manager.h"

//Archivos del sistema
#include "gpsManager.h"
#include "../UI/interfazUSB.h"
#include "../UI/interfazConversiones.h"
#include "../System/menu.h"
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Datos Globales">
SemaphoreHandle_t tramaValida;
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Funciones de interfaz">
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
// </editor-fold>

