/* ************************************************************************** */

#include "interfazPrincipal.h"

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

void interfazUSB(void* params) {
    uint8_t buffer[4];
    uint16_t numBytes;
    
    while (1) {
        if (!dispositivo.midiendo && USBUSARTIsTxTrfReady()) {
            numBytes = getsUSBUSART(buffer, sizeof (buffer));
            if (numBytes > 0) {
                if (!dispositivo.inicializado){
                    inicializar();
                }
                menu();
            }
        }
    }
}

boolean pedirID(){ // sin terminar
    
    uint8_t entrada[32]; // Array donde se va a recibir la entrada
    buscarEntrada(entrada,sizeof(entrada));
    
    
    
    dispositivo.dispositivoID = entrada;
       
}

boolean pedirNumeroDeContacto(){ // sin terminar
    uint8_t entrada[8];
    buscarEntrada(entrada,sizeof(entrada));
    
    
    
    dispositivo.numeroDeContacto = entrada;
}

boolean pedirTemperatura(){ 
    uint8_t entrada[2];
    uint8_t temperatura;
    buscarEntrada(entrada,sizeof(entrada));
    temperatura = ((10*entrada[0]) + (entrada[1] - ASCII_TO_INT_DIFFERENCE));
    dispositivo.umbralDeTemperatura = temperatura;
    
    
}


/* *****************************************************************************
 End of File
 */
