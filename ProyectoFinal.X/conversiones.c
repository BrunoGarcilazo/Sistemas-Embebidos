/* ************************************************************************** */
/** Descriptive File Name

  @Company
    UCUDAL

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



void conversiones(void *p_params){
    
    uint8_t contador;
    uint16_t promedio;
    uint16_t muestra;
    
    contador = 0;
    promedio = 0;
    muestra = 0; 
    while(contador != 10 ){ // && midiendo == true
        
        while(!ADC1_IsConversionComplete());
        
        muestra = ADC1_ConversionResultGet();
        samplesConversiones[contador] = muestra;
        contador = contador + 1;
        invertirLedsMedicion();
        vTaskDelay(pdMS_TO_TICKS(250));             
    }    
    // if(midiendo == false){ terminar tarea }
    
}

/** Hace el Promedio de todos los Samples obtenidos del ADC*/

uint16_t promedioSamples(){
    uint8_t i;
    uint16_t sumaDeValores;
    sumaDeValores = 0;
    
    
    for(i = 0; i < sizeof(samplesConversiones) ; i++){
        sumaDeValores = sumaDeValores + samplesConversiones[i];
    }
    
    return (sumaDeValores/sizeof (samplesConversiones));
}
    
/** Convierte el Promedio de los datos del ADC a una Temperatura en Celsius entre 32C y 42C*/
uint16_t conversorADCTemp(uint16_t promedio){
    
    
}
/* *****************************************************************************
 End of File
 */
