
/**
  Section: Included Files
 */

#include "utilss/utils.h"
#include "platform/WS2812.h"
#include "platform/usbManager.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/usb/usb_device_cdc.h"
#include "mcc_generated_files/rtcc.h"
#include "platform/scheludeManager.h" 
#include <time.h>
#include"platform/rtcManager.h"

/*
                         Main application
 */
bool inicializarLedsRGB() {
    ws2812_t leds[8]; // Array que luego voy a mandar con los colores que quiero
    ws2812_t amarillo; // struct de RGB que voy a modificar
    amarillo.r = 255;
    amarillo.g = 255; // Asigno el codigo que quiero
    amarillo.b = 0;
    int i;
    for (i = 0; i < 8; i++) {
        leds[i] = amarillo; // Asigno el color amarillo a los 8 lugares del Array
    }
    WS2812_send(leds, 8); // Envio los colores a los LEDs

    return true;
}

int main(void) {
    // initialize the device
    SYSTEM_Initialize();
    inicilizarEventos();
    aplicacion_t estado;
    estado.status = EN_ESPERA;

    //Variables de luces
    bool luzPrendida = false;
    uint32_t ochociento = 800;
    uint32_t cuatrociento = 400;
    ut_tmrDelay_t timer;
    timer.state = UT_TMR_DELAY_INIT;

    //Variables de USB
    uint8_t large = 10;
    uint8_t buffer[large];
    char menu[] = "\r\nBienvenido\r\nMenu\r\nPara seleccionar una opcion envie la tecla presente a la izquierda de la funcion\r\na - Fijar hora del reloj RTC\r\nb - Consultar hora del RTC\r\nc - Agregar evento\r\nd - Quitar evento\r\ne - Consultar lista de eventos\r\n";
    uint8_t numBytes;
    bool primeraVez;
    primeraVez = true;
    bool primeraVezMensajes;
    primeraVezMensajes = true;

    //Variables para input de usuario

    inicializador_t init;
    init.estado = MENSAJE_DE_FECHA_NO_ENVIADO;

    manager_de_pedidos_t manager;
    manager.estado = PEDIDO_INVALIDO;

    struct tm tiempoParaAplicaciones;


    while (1) {    
        //Maquina de estado UNO
        switch (estado.status) {
            case(EN_ESPERA):
                CDCTxService();
                if ((USBGetDeviceState() < CONFIGURED_STATE) ||
                        (USBIsDeviceSuspended() == true)) {
                    //Either the device is not configured or we are suspended
                    //so we don't want to do execute any application code
                    //go back to the top of the while loop
                    continue;
                } else {
                    if (USBUSARTIsTxTrfReady()) {
                        numBytes = getsUSBUSART(buffer, large);
                        if (numBytes > 0) {
                            if (primeraVez) {
                                putrsUSBUSART("\r\nInicializacion\r\n");
                                estado.status = NO_INICIALIZADA;
                                primeraVez = false;
                            } else {
                                switch (buffer[0]) { //Menu
                                    case('m'):
                                        if (buffer[1] == 'e' & buffer[2] == 'n' & buffer[3] == 'u') {
                                            estado.status = EN_MENU;
                                        }
                                        break;
                                    case('a'):
                                        estado.status = EN_INGRESAR_HORA;
                                        break;
                                    case('b'):
                                        estado.status = EN_MOSTRAR_HORA;
                                        break;
                                    case ('c'):
                                        estado.status = EN_AGREGAR_EVENTO;
                                        break;
                                    case ('d'):
                                        estado.status = EN_QUITAR_EVENTO;
                                        break;
                                    case ('e'):
                                        estado.status = EN_CONSULTAR_LISTA_DE_EVENTOS;
                                        break;
                                    default:
                                        break;
                                }
                                cleanBuffer(buffer);
                            }
                        }
                    }
                }
            case(NO_INICIALIZADA):
                if (inicializarFechaYHora(&init, &manager)) {
                    manager.estado = PEDIDO_INVALIDO;
                    estado.status = EN_MENU;
                }
                break;
            case(EN_MENU):
                if (enviarMensaje(menu)) {
                    estado.status = EN_ESPERA;
                }
                break;
            case(EN_INGRESAR_HORA):
                if (primeraVezMensajes == true) {
                    if (enviarMensaje("\r\nIngrese hora en formato hh:mm:ss\r\n")) {
                        primeraVezMensajes = false;
                    }
                }
                if (pedirHora(&tiempoParaAplicaciones, &manager)) {
                    RTCC_TimeSet(&tiempoParaAplicaciones);
                    estado.status = EN_MENU;
                }
                break;
            case(EN_MOSTRAR_HORA):
                if (consultarHora()) {
                    estado.status = EN_MENU;
                }
                break;
            case(EN_AGREGAR_EVENTO):
                break;
            case(EN_QUITAR_EVENTO):
                quitarEvento();
                break;
            case(EN_CONSULTAR_LISTA_DE_EVENTOS):
                consultarListaDeEventos();
                break;
            default:
                break;
        }




        switch (luzPrendida) {
            case false:
                if (UT_delayms(&timer, ochociento)) {
                    luzPrendida = true;
                    LEDB_SetHigh();

                }
                break;
            case true:
                if (UT_delayms(&timer, cuatrociento)) {
                    luzPrendida = false;
                    LEDB_SetLow();                 
                }
                break;
        }

        //Maquina de estado tres
        struct tm tiempoActual;
        int i;

        ws2812_t ledsRGB[8];
        for (i = 0; i < EVENTOS_MAXIMOS - 1; i++) {
            if (eventos[i].command != 0xFF) {
                RTCC_TimeGet(&tiempoActual);
                uint32_t tiempoActualPlano = mktime(&tiempoActual);
                if (eventos[i].time >= tiempoActualPlano) {

                    switch (eventos[i].color) {
                        case 0:
                            ledsRGB[eventos[i].param] = WHITE;
                            break;
                        case 1:
                            ledsRGB[eventos[i].param] = RED;
                            break;
                        case 2:
                            ledsRGB[eventos[i].param] = BLUE;
                            break;
                        case 3:
                            ledsRGB[eventos[i].param] = GREEN;
                        default:
                            break;
                    }

                }
            }
        }
        WS2812_send(ledsRGB, 8);



    }
}


