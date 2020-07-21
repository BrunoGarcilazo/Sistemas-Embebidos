/* ************************************************************************** */
/** Conversiones

  @Company
    UCUDAL

  @File Name
    conversiones.c

  @Summary
    Se encarga de las mediciones de temperatura del dispositivo

  @Description
    Mide, promedia, convierte la medida a grados. Prende leds cuando esta midiendo y luego leds de colores 
    segun la salud del paciente
 */
/* ************************************************************************** */
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "FreeRTOS.h"
#include "task.h"
#include "../freeRTOS/include/semphr.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "../mcc_generated_files/usb/usb_device_cdc.h"
#include "../mcc_generated_files/pin_manager.h"

#include "conversiones.h"
#include "../Platform/ledManager.h"
#include "../Platform/gpsManager.h"
#include "../UI/interfazConversiones.h"
#include "../UI/interfazUSB.h"
#include "../Communications/GPS.h"
#include "../Communications/SIM808.h"

xSemaphoreHandle prenderYapagarLucesRojas;
xSemaphoreHandle prenderYapagarLucesVerdes;

/**
 * Hace el Promedio de todos los Samples obtenidos del ADC
 * @return 
 */
float promedioSamples(uint16_t *samplesConversiones, uint8_t tamanio) {
    uint8_t i;
    uint16_t sumaDeValores;
    sumaDeValores = 0;
    for (i = 0; i < tamanio; i++) {
        sumaDeValores = sumaDeValores + samplesConversiones[i];
    }
    return sumaDeValores / tamanio;
}

/**
 * Convierte el Promedio de los datos del ADC a una Temperatura en Celsius entre 32C y 42C
 * @param promedio Promedio de las temperaturas medidas por el ADC
 * @return Valor en grados
 */
float conversorADCTemp(float promedio) {
    uint8_t vecesQueEntra10;
    float temperatura;

    //Corregir numeros magicos
    vecesQueEntra10 = floorf(promedio / 10);
    temperatura = 32 + (0.1 * vecesQueEntra10);
    return temperatura;
}

void alertarPersona(medida_t *medida) {
    uint8_t mensaje[100];
    uint8_t idDeDispositivo[10];
    uint8_t posicionStr[55];
    uint8_t fechaYhora[24];
    uint8_t temperaturaStr[5];
    struct tm *tiempoDeMedida;


    sprintf(idDeDispositivo, "%d", dispositivo.dispositivoID);
    strcpy(mensaje, idDeDispositivo);
    strcat(mensaje, " ");

    sprintf(temperaturaStr, "%.1f", medida->temperaturaRegistrada);
    strcat(mensaje, temperaturaStr);
    strcat(mensaje, " ");

    tiempoDeMedida = gmtime(&medida->tiempo);

    strftime(fechaYhora, sizeof (fechaYhora), " %d/%m/%Y-%H:%M ", tiempoDeMedida);
    strcat(mensaje, fechaYhora);

    GPS_generateGoogleMaps(posicionStr, medida->posicion);
    strcat(mensaje, posicionStr);

    SIM808_sendSMS(dispositivo.numeroDeContacto, mensaje);
}

void conversiones(void *p_params) {
    uint8_t conversionesAHacer = 10;
    uint16_t samplesConversiones[conversionesAHacer];
    uint8_t contador;
    uint16_t muestra;
    float promedio;

    GPSPosition_t posicion;
    medida_t medida;
    
    contador = 0;

    prenderYapagarLucesRojas = xSemaphoreCreateBinary();
    prenderYapagarLucesVerdes = xSemaphoreCreateBinary();
    
    
    xTaskCreate(prenderLedsRojosPor2Seg, "LucesRojas", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 6, NULL);
    xTaskCreate(prenderLedsVerdesPor2Seg, "LucesVerdes", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 6, NULL);

    while (1) {
        if (tramaValida != NULL && puedoMedir != NULL) {
            if (xSemaphoreTake(tramaValida, 0) == pdTRUE &&
                    xSemaphoreTake(puedoMedir, 0) == pdTRUE) {

                xSemaphoreGive(puedoMedir);
                while (contador != 10) {
                    if (xSemaphoreTake(puedoMedir,0) == pdTRUE) {
                        xSemaphoreGive(puedoMedir);
                        while (!ADC1_IsConversionComplete()) {
                        }
                        muestra = ADC1_ConversionResultGet();
                        samplesConversiones[contador] = muestra;
                        contador = contador + 1;
                        invertirLedsMedicion();
                        vTaskDelay(pdMS_TO_TICKS(250));
                    } else {
                        apagarLeds();
                        break;
                    }
                }
                contador = 0;
                if (xSemaphoreTake(puedoMedir,0) == pdTRUE) {
                    xSemaphoreGive(puedoMedir);
                    promedio = promedioSamples(samplesConversiones, conversionesAHacer);
                    promedio = conversorADCTemp(promedio);
                    
                    GPS_getPosition(&posicion, dispositivo.trama);

                    medida.tiempo = mktime(&tiempoDelSistema);

                    medida.posicion = posicion;
                    medida.temperaturaRegistrada = promedio;

                    mediciones[ultimaMedida] = medida;
                    ultimaMedida++;

                    if (xSemaphoreTake(puedoMedir,0) == pdTRUE) {
                        if (promedio > dispositivo.umbralDeTemperatura) {
                            xSemaphoreGive(prenderYapagarLucesRojas);
                            alertarPersona(&medida);
                        } else {
                            xSemaphoreGive(prenderYapagarLucesVerdes);
                        }
                    } else {
                        apagarLeds();
                        continue;
                    }
                } else {
                    continue;
                }
                xSemaphoreGive(inicializado);
                xSemaphoreGive(tramaValida);
            } else {
                vTaskDelay(pdMS_TO_TICKS(30));
            }
        } else {
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}

/* *****************************************************************************
 End of File
 */
