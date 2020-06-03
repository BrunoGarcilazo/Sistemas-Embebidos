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


#include "menu.h"
#include "../mcc_generated_files/usb/usb_device_cdc.h"
#include "../Platform/usbManager.h"
#include "../Platform/rtcManager.h"
#include "scheludeManager.h"

// *****************************************************************************
// *****************************************************************************
// Section: Funciones de interfaz
// *****************************************************************************
// *****************************************************************************

void menu(void *p_params) {
    bool primeraVez = true;
    MENU_STATUS status;
    status = EN_MENU;
    uint8_t buffer[4];
    uint8_t numBytes;
    while (true) {

        switch (status) {
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
                            inicializar();
                            primeraVez = false;
                            status = EN_MENU;
                        } else {
                            switch (buffer[0]) {
                                case('m'): //Los estados indican que significa cada opcion
                                    if (buffer[1] == 'e' & buffer[2] == 'n' & buffer[3] == 'u') {
                                        status = EN_MENU;
                                    }
                                    break;
                                case('a'):
                                {
                                    struct tm tiempoASetear;
                                    pedirHora(&tiempoASetear);
                                    RTCC_TimeSet(&tiempoASetear);
                                    status = EN_MENU;
                                    break;
                                }
                                case('b'):
                                    mostrarHora();
                                    status = EN_MENU;
                                    break;
                                case ('c'):
                                    agregarEvento();
                                    status = EN_MENU;
                                    break;
                                case ('d'):
                                    quitarEvento();
                                    status = EN_MENU;
                                    break;
                                case ('e'):
                                    consultarListaDeEventos();
                                    status = EN_MENU;
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                }
                break;
            default:
                break;
        }
    }
}

/* *****************************************************************************
 End of File
 */
