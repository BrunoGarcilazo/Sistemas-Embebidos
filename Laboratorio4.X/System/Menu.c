/* ************************************************************************** */
/** Menu

  @Company
    UCUDAL

  @File Name
    Menu.c

  @Summary
    Contiene la tarea menu y las variables asociadas 

  @Description
    La tarea menu muestra el menu y espera por un input.
    Cuando el input es valido se crea una funcion.
 */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Archivos incluidos                                                */
/* ************************************************************************** */
/* ************************************************************************** */
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include "FreeRTOS.h"
#include "task.h"


#include "Menu.h"
#include "../mcc_generated_files/usb/usb_device_cdc.h"
#include "../Platform/usbManager.h"
#include "../Platform/rtcManager.h"

// *****************************************************************************
// *****************************************************************************
// Section: Funciones de interfaz
// *****************************************************************************
// *****************************************************************************

void menu(void *p_params) {
    bool primeraVez = true;
    MENU_STATUS status;
    status = EN_MENU;
    while (true) {
        uint8_t buffer[4];
        uint8_t numBytes;
        switch (status) {
            case(NO_INICIALIZADA):
                vTaskPrioritySet(NULL, tskIDLE_PRIORITY + 1);
                xTaskCreate(inicializar, "Inicialize", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
                vTaskPrioritySet(NULL, tskIDLE_PRIORITY + 2);
                status = EN_MENU;
                break;
            case(EN_MENU):
                CDCTxService(); //Revisar si no se puede poner afuera
                if ((USBGetDeviceState() < CONFIGURED_STATE) || //Probar si se puede sacar
                        (USBIsDeviceSuspended() == true)) {

                } else {
                    if (USBUSARTIsTxTrfReady()) {
                        putsUSBUSART(MENU); //Si pudo enviar el mensaje devuelve 
                        status = EN_ESPERA;
                    }
                }
                break;
            case(EN_ESPERA):
                CDCTxService(); //Revisar si no se puede poner afuera 
                if (USBUSARTIsTxTrfReady()) {
                    numBytes = getsUSBUSART(buffer, sizeof buffer);
                    if (numBytes > 0) {
                        if (primeraVez) {
                            status = NO_INICIALIZADA;
                            primeraVez = false;
                        }
                        switch (buffer[0]) {
                            case('m'): //Los estados indican que significa cada opcion
                                if (buffer[1] == 'e' & buffer[2] == 'n' & buffer[3] == 'u') {
                                    status = EN_MENU;
                                }
                                break;
                            case('a'):
                                status = EN_INGRESAR_HORA;
                                break;
                            case('b'):
                                status = EN_MOSTRAR_HORA;
                                break;
                            case ('c'):
                                status = EN_AGREGAR_EVENTO;
                                break;
                            case ('d'):
                                status = EN_QUITAR_EVENTO;
                                break;
                            case ('e'):
                                status = EN_CONSULTAR_LISTA_DE_EVENTOS;
                                break;
                            default:
                                break;
                        }
                    }
                }
                break;
            case(EN_MOSTRAR_HORA):
                //To do
                //Crear tarea mostrar hora
                status = EN_MENU;
                break;
            case(EN_INGRESAR_HORA):
            {
                struct tm tiempo;
                pedirHora(&tiempo);
                RTCC_TimeSet(&tiempo);
                status = EN_MENU;
                break;
            }
            case(EN_AGREGAR_EVENTO):
                //To do
                //Crear tarea agregar evento
                status = EN_MENU;
                break;
            case(EN_QUITAR_EVENTO):
                //To do 
                //Crear tarea quitar evento
                status = EN_MENU;
                break;
            case(EN_CONSULTAR_LISTA_DE_EVENTOS):
                //To do 
                //Crear tarea consultar lista de eventos
                status = EN_MENU;
                break;
            default:
                break;
        }
    }
}

/* *****************************************************************************
 End of File
 */
