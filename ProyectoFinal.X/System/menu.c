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

//Includes de FreeRtos
#include "FreeRTOS.h"
#include "task.h"

//Librerias
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

//MCC
#include "../mcc_generated_files/usb/usb_device_cdc.h"

//Archivos del programa
#include "menu.h"
#include "../Platform/usbManager.h"
#include "../Platform/rtcManager.h"
#include "../UI/interfazUSB.h"

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

    bool temperaturaOk;
    temperaturaOk = false;
    bool telefonoOk;
    telefonoOk = false;
    bool idOk;
    idOk = false;

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
                    enviarMensaje("\r\nBienvenido\r\nEnvie la tecla presente a la izquierda de la funcion\r\n"); 
                    enviarMensaje("a - Set ID\r\nb - Set umbral de temperatura\r\n");  
                    enviarMensaje("c - Set telefono de contacto\r\nd - Imprimir medidas\r\n");  
                    enviarMensaje("e - Borrar registros\r\nf - Terminar\r\n");  
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
                            case ('a'): //Poner Id del dispositivo, numero unico de 32 bits.
                                while (!idOk) {
                                    enviarMensaje(PEDIDO_DE_ID);
                                    idOk = pedirID();
                                }
                                status = EN_MENU;
                                break;
                            case ('b'): //Umbral de temperatura
                                while (!temperaturaOk) {
                                    enviarMensaje(PEDIDO_DE_UMBRAL);
                                    temperaturaOk = pedirTemperatura();   
                                }
                                status = EN_MENU;
                                break;
                            case ('c')://Telefono para enviar mensajes
                                while (!telefonoOk) { // HAY QUE PROBARLO
                                    enviarMensaje(FORMATO_DE_TELEFONO);
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
                            case ('d')://Imprimir lista de medidas 
                                status = EN_MENU;
                                break;
                            case ('e')://Borrar medidias 
                                status = EN_MENU;
                                break;
                            case ('f')://Terminar conexion
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
