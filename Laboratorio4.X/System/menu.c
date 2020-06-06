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
    uint8_t buffer[4];
    uint8_t numBytes;
    struct tm tiempoASetear;
    
    status = EN_MENU;

    while (true) {
        CDCTxService();
        switch (status) {
            case(EN_MENU):
                if ((USBGetDeviceState() < CONFIGURED_STATE) ||
                        (USBIsDeviceSuspended() == true)) {

                } else {
                    if (USBUSARTIsTxTrfReady()) {
                        putsUSBUSART(MENU); //Si se pudo enviar el menu 
                        status = EN_ESPERA; //Se espera un input
                    }
                }
                break;
            case(EN_ESPERA):
                if (USBUSARTIsTxTrfReady()) {
                    numBytes = getsUSBUSART(buffer, sizeof (buffer));
                    if (numBytes > 0) { // Si recibimos un input
                        if (primeraVez) { //La primera vez inicializamos fecha y hora
                            inicializar();
                            primeraVez = false;
                            status = EN_MENU; //Luego imprimimos el menu
                        } else {
                            switch (buffer[0]) {
                                case('m'): //Los estados indican que significa cada opcion
                                    if (buffer[1] == 'e' & buffer[2] == 'n' & buffer[3] == 'u') {
                                        status = EN_MENU; //Imprimimos menu
                                    }
                                    break;
                                case('a'):
                                {
                                    pedirHora(&tiempoASetear); //Pedimos que se ingrese una hora
                                    pedirFecha(&tiempoASetear); //Pedimos que se ingrese una fecha
                                    RTCC_TimeSet(&tiempoASetear); //Seteamos el RTC a esa hora
                                    status = EN_MENU; //Pasamos al menu
                                    break;
                                }
                                case('b'):
                                    mostrarHora(); //Muestra la hora del sistema
                                    status = EN_MENU; 
                                    break;
                                case ('c'):
                                    agregarEvento(); //Agrega un evento al array de eventos
                                    status = EN_MENU; 
                                    break;
                                case ('d'):
                                    quitarEvento(); //Quita un evento del array de eventos
                                    status = EN_MENU;
                                    break;
                                case ('e'):
                                    consultarListaDeEventos(); //Imprime todos los eventos
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
