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

#include "../Platform/ledManager.h"
#include "../Platform/rtcManager.h"
#include "conversiones.h"
#include "../UI/interfazConversiones.h"
#include "../Communications/GPS.h"
#include "../Communications/SIM808.h"

/**
 * Hace el Promedio de todos los Samples obtenidos del ADC
 * @return 
 */
uint16_t promedioSamples(uint16_t *samplesConversiones) {
    uint8_t i;
    uint16_t sumaDeValores;
    sumaDeValores = 0;
    for (i = 0; i < sizeof (samplesConversiones) / sizeof (samplesConversiones[0]); i++) {
        sumaDeValores = sumaDeValores + samplesConversiones[i];
    }
    return sumaDeValores / (sizeof (samplesConversiones) / sizeof (samplesConversiones[0]));
}

/**
 * Convierte el Promedio de los datos del ADC a una Temperatura en Celsius entre 32C y 42C
 * @param promedio Promedio de las temperaturas medidas por el ADC
 * @return Valor en grados
 */
uint16_t conversorADCTemp(float promedio) {
    uint8_t vecesQueEntra10;
    float temperatura;

    //Corregir numeros magicos
    vecesQueEntra10 = floorf(promedio / 10);
    temperatura = 32 + (0.1 * vecesQueEntra10);
    return temperatura;
}

void alertarPersona(void *p_params) {
    GPSPosition_t posicion;
    uint8_t mensaje[100];
    uint8_t idDeDispositivo[10];
    uint8_t posicionStr[55];
    uint8_t fechaYhora[24];
    uint8_t temperaturaStr[4];
    float temperatura;
    float *temperaturaPaciente;
    temperaturaPaciente = (float*) p_params;
    temperatura = temperaturaPaciente[0];

    if (xSemaphoreTake(horaSeteada, portMAX_DELAY) == pdTRUE) {
        sprintf(idDeDispositivo, "%d", dispositivo.dispositivoID);
        strcpy(mensaje, idDeDispositivo);
        strcat(mensaje, " ");

        sprintf(temperaturaStr, "%f", temperatura);
        strcat(mensaje, temperaturaStr);
        strcat(mensaje, " ");

        RTCC_TimeGet(&tiempoDelSistema);
        strftime(fechaYhora, sizeof (fechaYhora), " %d/%m/%Y-%H:%M ", &tiempoDelSistema);
        strcat(mensaje, fechaYhora);

        GPS_getPosition(&posicion, dispositivo.trama);
        GPS_generateGoogleMaps(posicionStr, posicion);
        strcat(mensaje, posicionStr);
        SIM808_sendSMS(dispositivo.numeroDeContacto, mensaje);
    }
}

void conversiones(void *p_params) {
    uint16_t samplesConversiones[10];
    uint8_t contador;
    float promedio;
    float promedioParaMensaje[1];
    uint16_t muestra;
    medida_t medida;

    while (!ADC1_IsConversionComplete()) {
    }
    muestra = ADC1_ConversionResultGet();

    contador = 0;
    while (contador != 10) {
        while (!ADC1_IsConversionComplete()) {
        }
        muestra = ADC1_ConversionResultGet();
        samplesConversiones[contador] = muestra;
        contador = contador + 1;
        invertirLedsMedicion();
        vTaskDelay(pdMS_TO_TICKS(250));
    }
    promedio = promedioSamples(samplesConversiones);
    promedio = conversorADCTemp(promedio);
    if (promedio > dispositivo.umbralDeTemperatura) {
        promedioParaMensaje[0] = promedio;
        xTaskCreate(prenderLedsRojosPor2Seg, "LucesRojas", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 6, NULL);
        xTaskCreate(alertarPersona, "MandarMensaje", configMINIMAL_STACK_SIZE + 500, (void*) promedioParaMensaje, tskIDLE_PRIORITY + 4, NULL);
    } else {
        xTaskCreate(prenderLedsVerdesPor2Seg, "LucesVerdes", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 6, NULL);
    }

    medida.temperaturaRegistrada = promedio;
    mediciones[ultimaMedida] = medida;
    ultimaMedida++;

    dispositivo.midiendo = false;

    vTaskDelay(pdMS_TO_TICKS(2000));
    apagarLeds();

    vTaskDelete(NULL);
}


/* *****************************************************************************
 End of File
 */
