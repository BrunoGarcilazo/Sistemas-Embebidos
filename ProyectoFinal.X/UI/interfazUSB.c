/**InterfazUSB

  @Company
    UCUDAL

  @File Name
    interfazUSB.c

  @Summary
    Gestion de la interaccion via USB de la placa

  @Description
    Tareas y funciones que se encargan del ingreso y salida de datos via USB
 */

// <editor-fold defaultstate="collapsed" desc="Includes">
//FreeRtos
#include  "FreeRTOS.h"
#include  "task.h"

//MCC
#include  "../mcc_generated_files/usb/usb_device_cdc.h"

//Librerias
#include <stdint.h>
#include <string.h>

//Archivos del sistema
#include "interfazUSB.h"
#include "../System/menu.h"
#include "interfazConversiones.h"
#include "../System/conversiones.h"
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Datos Globales">

SemaphoreHandle_t inicializado;
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Funciones de Interfaz">

/**
 * Esta funcion inicializa los datos imprescindibles para hacer una medida pidiendolos via USB
 */
void inicializar() {
    pedirID(); //Pide ID al ususario via USB
    pedirUmbralTemperatura(); //Pide umbral de temperatura al usuario via USB
    pedirNumeroDeContacto(); //Pide numero de contacto al usuario via USB
}

/**
 * Tarea que se encarga de reaccionar ante el ingreso de datos por la consola, chequea una vez por segundo
 * Si el dispositivo no esta inicializado ejecuta la funcion de inicializacion via USB y libera un semaforo
 * que permite realizar otras actividades del sistema.
 * Si el dispositivo esta inicializado ejecuta el menu si no se esta haciendo una conversion
 * @param params
 */
void interfazUSB(void* params) {
    inicializado = xSemaphoreCreateBinary(); //Crea el semaforo de incializado
    uint8_t buffer[4]; //Buffer de ingreso de datos
    uint8_t numBytes; //Numero de bytes ingresados
    while (1) {
        if ((USBGetDeviceState() >= CONFIGURED_STATE) && !USBIsDeviceSuspended()) {
            numBytes = getsUSBUSART(buffer, strlen(buffer)); //Verificamos si se recibio algo
            if (numBytes > 0) { //Si se recibio algo
                if (!dispositivo.inicializado) { //Si no se ha inicializado el dispositivo
                    inicializar(); //Ejecutamos la funcion de inicializacion
                    if (dispositivo.numeroDeContacto[2] == '9' && dispositivo.umbralDeTemperatura != 0
                            && dispositivo.dispositivoID != 0) { //Si se inicializaron todos los valores
                        dispositivo.inicializado = true; //El dispositivo queda como inicializado
                        xSemaphoreGive(inicializado); //Liberamos el semaforo de inicializado
                    }
                } else {
                    if (xSemaphoreTake(inicializado, 0) == pdTRUE) { //Semaforo libre = puedo ejecutar el menu
                        menu(); //Ejecutamos el menu
                        xSemaphoreGive(inicializado); //Liberamos el semaforo
                    }
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/**
 * Ejecuta CDCTXService() cada 10 ms
 * @param p_param
 */
void mantenimientoUSB(void * p_param) {
    while (1) {
        if ((USBGetDeviceState() >= CONFIGURED_STATE) && !USBIsDeviceSuspended()) {
            CDCTxService();
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
// </editor-fold>

