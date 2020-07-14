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

void menu() {
    MENU_STATUS status;
    uint8_t buffer[4];
    uint8_t numBytes;
    struct tm tiempoASetear;
    bool telefonoOk;
    telefonoOk = false;
    status = EN_MENU;


    /**
     * Lista de menú:
     * Setear hora
     * Mostrar hora
     * Poner ID de dispositivo
     * Umbral de temperatura
     * Telefono para enviar mensajes
     * Imprimir lista de medidas
     * Borrar todos las medidas
     * Terminar conexion
     */
    while (true) {
        if (USBUSARTIsTxTrfReady()) {
            switch (status) {
                case(EN_MENU):
                    putsUSBUSART(MENU); //Si se pudo enviar el menu 
                    status = EN_ESPERA; //Se espera un input
                    break;
                case(EN_ESPERA):
                    numBytes = getsUSBUSART(buffer, sizeof (buffer));
                    if (numBytes > 0) { //Si recibimos un input
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
                            case ('c'): //Poner Id del dispositivo, numero unico de 32 bits.
                                pedirID();
                                status = EN_MENU;
                                break;
                            case ('d'): //Umbral de temperatura
                                pedirTemperatura();
                                status = EN_MENU;
                                break;
                            case ('e')://Telefono para enviar mensajes
                                while(!telefonoOk){ // HAY QUE PROBARLO
                                    enviarMensaje("Ingrese numero de telefono celular. Formato: 096123456");
                                    telefonoOk = pedirNumeroDeContacto();
                                }               
                                /** Otra forma
                                 enviarMensaje("Ingrese numero de telefono celular. Formato: 096123456");
                                 telefonoOk = pedirNumeroDeContacto();
                                 if(!telefonoOk){
                                   buffer[0] == 'e';
                                   status = EN_ESPERA; 
                                 }else{
                                   status = EN_MENU
                                 
                                 }
                                 * 
                                 
                                 */
                                status = EN_MENU;
                                break;
                            case ('f')://Imprimir lista de medidas 
                                status = EN_MENU;
                                break;
                            case ('g')://Borrar medidias 
                                status = EN_MENU;
                                break;
                            case ('h')://Terminar conexion
                                vTaskDelete(NULL);
                                break;
                            default:
                                break;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

/* *****************************************************************************
 End of File
 */
