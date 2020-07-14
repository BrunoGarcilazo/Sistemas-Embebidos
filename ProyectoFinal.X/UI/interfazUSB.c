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
#include "../System/conversiones.h"

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
                xTaskCreate(conversiones, "Conversiones", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 5, &conversionHandler);
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
/**
 * 
 * @return numero dado es valido o no
 */
bool validarTelefono(char numero[9]){
    int i;
    if((numero[0] - ASCII_TO_INT_DIFFERENCE) == 0){
        if((numero[1] - ASCII_TO_INT_DIFFERENCE) == 9){
            for(i=0;i<9;i++){
                if((numero[i] - ASCII_TO_INT_DIFFERENCE) >= 10 || (numero[i] - 48) < 0){
                    return false; // No es un numero
                }
            }
             // Valide todos los numeros
            return true;         
        }else{
            return false; // No es de la manera 09XXXXXX
        }
    }else{
        return false; // No comienza con 0
    }
    
}

bool pedirNumeroDeContacto(){ // sin terminar
    int i,k;
    char entrada[9];
    uint8_t numeroSolo[9];
    uint8_t numeroFormatoSMS[11];
    
    memset(entrada, 0, (sizeof(entrada)/entrada[0])); //Se limpia la entrada
    buscarEntrada(entrada, (sizeof(entrada)/entrada[0]));
    
    if(validarTelefono(entrada)){
        // Convierto la entrada en un array de char con el formato \"092020400\"
        numeroFormatoSMS[0] = '"';
        for(i=0;i<sizeof(entrada);i++){
            numeroFormatoSMS[i+1] = entrada[i];
        }
        numeroFormatoSMS[10] = '"';
        // Ahora numeroFormatoSMS = "096123456"
        for(k=0;k<sizeof(entrada);k++){
            dispositivo.numeroDeContacto[k] = numeroFormatoSMS[k];
        }
        return true;
    }else{
        return false;
    }
}



/**
 * 
 */
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
