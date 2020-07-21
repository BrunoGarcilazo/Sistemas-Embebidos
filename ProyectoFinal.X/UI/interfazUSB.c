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
#include <math.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


#include "../System/conversiones.h"
#include "interfazUSB.h"
#include "interfazConversiones.h"
#include "../Platform/rtcManager.h"
#include "../System/utils.h"

void interfazUSB(void* params) {
    uint8_t buffer[4];
    uint16_t numBytes;
    while (1) {
        if (USBUSARTIsTxTrfReady() & (USBGetDeviceState() >= CONFIGURED_STATE) && !USBIsDeviceSuspended()) {
            numBytes = getsUSBUSART(buffer, strlen(buffer));
            if (numBytes > 0) {
                /*if (!dispositivo.inicializado) {
                    inicializar();
                }*/
                menu();
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void mantenimientoUSB(void * p_param) {
    while (1) {
        if ((USBGetDeviceState() >= CONFIGURED_STATE) && !USBIsDeviceSuspended()) {
            CDCTxService();
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }

}

/**
 * Pide al Usuario un ID para el Dispositivo
 * @return si el ID ingresado se guardo con exito o no (fue valido)
 */
bool pedirID() { // sin terminar
    uint32_t k;
    uint8_t i;
    char entrada[10]; // Array donde se va a recibir la entrada
    memset(entrada, 0, sizeof (entrada)); //Se limpia la entrada
    buscarEntrada(entrada, sizeof (entrada));
    for (i = 0; i < 10; i++) { // Metodo de conversion de Array de char a int : shorturl.at/acoAL
        if ((entrada[i] - ASCII_TO_INT_DIFFERENCE) >= 10 || (entrada[i] - ASCII_TO_INT_DIFFERENCE) < 0) { // Solo puede ser numerico
            return false;
        }
        k = 10 * k + (entrada[i] - ASCII_TO_INT_DIFFERENCE);
    }
    if (k > 4294967295 || k < 0) {
        return false;
    } else {
        dispositivo.dispositivoID = k;
    }
    return true;
}

/**
 * 
 * @return numero dado es valido o no
 */
bool validarTelefono(char *numero) {
    uint8_t i;
    if ((numero[0] - ASCII_TO_INT_DIFFERENCE) == 0) {
        if ((numero[1] - ASCII_TO_INT_DIFFERENCE) == 9) {
            for (i = 0; i < 9; i++) {
                if ((numero[i] - ASCII_TO_INT_DIFFERENCE) >= 10 || (numero[i] - 48) < 0) {
                    return false; // No es un numero
                }
            }
            // Valide todos los numeros
            return true;
        } else {
            return false; // No es de la manera 09XXXXXX
        }
    } else {
        return false; // No comienza con 0
    }

}

/**
 * Funcion que pide un numero de contacto al Usuario
 * @return  Telefono ingresado valido o no
 */
bool pedirNumeroDeContacto() {
    uint8_t i, k;
    char entrada[9];
    uint8_t numeroSolo[9];
    uint8_t numeroFormatoSMS[11];

    memset(entrada, 0, (sizeof (entrada))); //Se limpia la entrada
    buscarEntrada(entrada, (sizeof (entrada)));

    if (validarTelefono(entrada)) {
        // Convierto la entrada en un array de char con el formato \"092020400\"
        numeroFormatoSMS[0] = '"';
        for (i = 0; i<sizeof (entrada); i++) {
            numeroFormatoSMS[i + 1] = entrada[i];
        }
        numeroFormatoSMS[10] = '"';
        // Ahora numeroFormatoSMS = "096123456"
        for (k = 0; k<sizeof (entrada); k++) {
            dispositivo.numeroDeContacto[k] = numeroFormatoSMS[k];
        }
        return true;
    } else {
        return false;
    }
}

/**
 * Funcion que verifica si una temperatura dada por el Usuario es valida o no
 * @param entrada dada por el Usuario
 * @return entrada valida o no
 */
bool validarTemperatura(char entrada[4]) {
    uint8_t parteEntera;
    parteEntera = ((10 * (entrada[0] - ASCII_TO_INT_DIFFERENCE)) + (entrada[1] - ASCII_TO_INT_DIFFERENCE));
    if (parteEntera > 42 || parteEntera < 32) {
        return false;
    } else {
        if (entrada[2] != '.') {
            return false;
        } else {
            if ((entrada[3] - ASCII_TO_INT_DIFFERENCE) < 0 || (entrada[3] - ASCII_TO_INT_DIFFERENCE) > 9) {
                return false;
            } else {
                return true;
            }
        }
    }
}

/**
 *  Funcion que pide el Umbral de Temperatura al Usuario
 * @return Temperatura valida o no
 */
bool pedirTemperatura() {
    bool verificado;
    uint8_t entrada[4];
    buscarEntrada(entrada, sizeof (entrada));
    verificado = validarTemperatura(entrada);
    if (verificado) {
        dispositivo.umbralDeTemperatura = atof(entrada);
        return true;
    } else {
        return false;
    }

}

/**
 * Funcion que envia por USB las medidas guardadas hasta el momento
 */
void imprimirMedidas(){
    char id[15];
    strcpy(id,"\r\nMedida ");
    
    char temperaturaMensaje[26];
    strcpy(temperaturaMensaje,"\r\nTemperatura: ");
    
    char fechaMensaje[40];
    strcpy(fechaMensaje,"\r\nFecha y Hora: ");
    
    char indiceStringID[3];
    
    char posicionString[55];
    char posicionMensaje[80];
    strcpy(posicionMensaje,"\r\nLocalizacion: ");
    
    uint8_t i;
    uint8_t fechaYhora[24];
    struct tm *tiempoMedida; 
    medida_t *medida;
    char temperatura[4];
    time_t timetMedida;
    for(i=0;i<ultimaMedida;i++){
        medida = &mediciones[i];
        
        sprintf(indiceStringID,"%d",i);
        strcat(id,indiceStringID);        
        
        timetMedida = mediciones[i].tiempo;
        tiempoMedida = gmtime(&timetMedida);
        strftime(fechaYhora,24, " %d/%m/%Y-%H:%M ",tiempoMedida);
        strcat(fechaMensaje,fechaYhora);
        
        
        ftoa(medida->temperaturaRegistrada,temperatura,1);
        strcat(temperaturaMensaje,temperatura);
             
        GPS_generateGoogleMaps(posicionString, mediciones[i].posicion);
        strcat(posicionMensaje,posicionString);
        strcat(posicionMensaje,"\r\n");
        
        
        enviarMensaje(id);
        enviarMensaje(fechaMensaje);
        enviarMensaje(temperaturaMensaje);
        enviarMensaje(posicionMensaje);
        
        //Coloco todas las variables en estados "inciciales" otra vez
        memset(temperatura, 0, sizeof temperatura);
        memset(fechaYhora,0,sizeof fechaYhora);
        memset(fechaMensaje,0,sizeof fechaMensaje);
        memset(posicionMensaje,0,sizeof posicionMensaje);
        memset(temperaturaMensaje,0,sizeof temperaturaMensaje);
        
        strcpy(id,"\r\nMedida ");
        strcpy(temperaturaMensaje,"\r\nTemperatura: ");
        strcpy(fechaMensaje,"\r\nFecha y Hora: ");
        strcpy(posicionMensaje,"\r\nLocalizacion: ");
        vTaskDelay(pdMS_TO_TICKS(1000));
        
    }
 
/* *****************************************************************************
 End of File
 */
}