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
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include "FreeRTOS.h"
#include "task.h"

#include "../ProyectoFinal.X/mcc_generated_files/usb/usb_device_cdc.h"
#include "../ProyectoFinal.X/Platform/usbManager.h"
#include "../ProyectoFinal.X/Platform/rtcManager.h"
#include "ledManager.h"
#include "conversiones.h"
#include "UI/interfazPrincipal.h"

/**
 * Hace el Promedio de todos los Samples obtenidos del ADC
 * @return 
 */
uint16_t promedioSamples() {
    uint8_t i;
    uint16_t sumaDeValores;
    sumaDeValores = 0;

    for (i = 0; i < sizeof (samplesConversiones); i++) {
        sumaDeValores = sumaDeValores + samplesConversiones[i];
    }

    return (sumaDeValores / sizeof (samplesConversiones));
}

/**
 * Convierte el Promedio de los datos del ADC a una Temperatura en Celsius entre 32C y 42C
 * @param promedio Promedio de las temperaturas medidas por el ADC
 * @return Valor en grados
 */
uint16_t conversorADCTemp(uint16_t promedio) {
    return promedio;
}

void conversiones(void *p_params) {
    tm tiempoActual;
    uint8_t contador;
    uint16_t promedio;
    uint16_t muestra;
    medida_t medida;

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
    promedio = promedioSamples();
    //promedio = conversorADCTemp(promedio);
    if (promedio > dispositivo.umbralDeTemperatura){
        xTaskCreate(prenderLedsRojosPor2Seg, "LucesRojas", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL);
        //Enviar mensaje por SMS
    } else {
        xTaskCreate(prenderLedsVerdesPor2Seg, "LucesVerdes", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL);
    }
    //Guardar medida
    
    medida.IdDelRegistro = ultimaMedida;
    medida.temperaturaRegistrada = promedio;
    if(RTCC_TimeGet(tiempoActual)){       
     medida.tiempo = tiempoActual;       
    }
 
    mediciones[ultimaMedida] = medida;
    ultimaMedida++;
    
    
}

/* *****************************************************************************
 End of File
 */
