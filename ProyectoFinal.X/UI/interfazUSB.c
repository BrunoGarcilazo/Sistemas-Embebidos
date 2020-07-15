/* ************************************************************************** */
/** InterfazUSB

  @Company
    UCUDAL

  @File Name
    interfazUSB.c

  @Summary
    Gestion de la interaccion del mundo exterior con la placa 

  @Description
    Crear tareas para manejar la interaccion de la placa con el mundo
 */

#include  "FreeRTOS.h"
#include  "task.h"

#include  "../mcc_generated_files/usb/usb_device_cdc.h"

#include  <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../System/conversiones.h"
#include "interfazUSB.h"
#include "interfazConversiones.h"


void interfazUSB(void* params) {
    uint8_t buffer[4];
    uint16_t numBytes;
    while (1) {
        if (USBUSARTIsTxTrfReady() & (USBGetDeviceState() >= CONFIGURED_STATE) && !USBIsDeviceSuspended()) {
            CDCTxService();
            numBytes = getsUSBUSART(buffer, strlen (buffer));
            if (numBytes > 0) {
                if (!dispositivo.inicializado){
                    inicializar();
                }
                menu();
            }
        }
    }
}

void mantenimientoUSB(void * p_param) {
    while (1) {
        if ((USBGetDeviceState() >= CONFIGURED_STATE) && !USBIsDeviceSuspended()) {
            CDCTxService();
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

bool pedirID() { // sin terminar
    uint32_t k;
    uint8_t i;
    char entrada[32]; // Array donde se va a recibir la entrada
    memset(entrada, 0, sizeof (entrada)); //Se limpia la entrada
    buscarEntrada(entrada, sizeof (entrada));
    for (i = 0; i < 32; i++) { // Metodo de conversion de Array de char a int : shorturl.at/acoAL
        if((entrada[i] - ASCII_TO_INT_DIFFERENCE) >= 10 || (entrada[i] - ASCII_TO_INT_DIFFERENCE) < 0){ // Solo puede ser numerico
            return false;
        }       
        k = 10 * k + (entrada[i] - ASCII_TO_INT_DIFFERENCE);
    }
    dispositivo.dispositivoID = k;
    return true;
}

/**
 * 
 * @return numero dado es valido o no
 */
bool validarTelefono(char numero[9]){
    uint8_t i;
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
    uint8_t i,k;
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

bool validarTemperatura(char entrada[4]){
    
    uint8_t parteEntera;
    parteEntera = ((10 * entrada[0] - ASCII_TO_INT_DIFFERENCE) + (entrada[1] - ASCII_TO_INT_DIFFERENCE));
    if(parteEntera > 42 || parteEntera < 32){
        return false;
    }else{
        if(entrada[2]!='.'){
            return false;
        }else{
            if((entrada[3] - ASCII_TO_INT_DIFFERENCE) < 0 || (entrada[3] - ASCII_TO_INT_DIFFERENCE) > 9){
                return false;
            }else{
                return true;
            } 
        }
    }
}
/**
 * 
 */
bool pedirTemperatura() {
    bool verificado;
    uint8_t entrada[4];
    float temperatura;
    buscarEntrada(entrada, sizeof (entrada));
    verificado = validarTemperatura(entrada);
    if(verificado){
        dispositivo.umbralDeTemperatura = atof(entrada);
        return true;
    }else{
        return false;
    }
    
}
/* *****************************************************************************
 End of File
 */
