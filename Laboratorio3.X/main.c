
/**
  Section: Included Files
 */

//Archivos de usuario
#include "utilss/utils.h"
#include "platform/WS2812.h"
#include "platform/usbManager.h"
#include "platform/scheludeManager.h"
#include"platform/rtcManager.h"

//Archivos generados por el mcc
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/usb/usb_device_cdc.h"
#include "mcc_generated_files/rtcc.h"

//Librerias
#include <time.h>



bool inicializarLedsRGB(ws2812_t *leds) {
    int i;
    for (i = 0; i < EVENTOS_MAXIMOS; i++) {
        leds[i] = BLACK; // Asigno el color apagado a los 8 lugares del Array
    }
    WS2812_send(leds, 8); // Envio los colores a los LEDs

    return true;
}


/*
 Main application
*/
int main(void) {
    // initialize the device
    SYSTEM_Initialize();
    
    //Inicializo el estado
    aplicacion_t estado;
    estado.status = EN_ESPERA;

    //Variables de luces
    bool luzPrendida = false;
    uint32_t ochociento = 800;
    uint32_t cuatrociento = 400;
    ut_tmrDelay_t timer;
    timer.state = UT_TMR_DELAY_INIT;
    
    //Luces RGB
    ws2812_t ledsRGB[EVENTOS_MAXIMOS];

    //Variables de USB
    uint8_t large = 10;
    uint8_t buffer[large];
    uint8_t numBytes;

    //Variables para ejecutar tareas

    bool primeraVez;
    primeraVez = true;

    bool primeraVezMensajes;
    primeraVezMensajes = true;

    inicializador_t init; //Se usa para inicializar el tiempo
    init.estado = MENSAJE_DE_FECHA_NO_ENVIADO;

    manager_de_pedidos_t manager; //Se usa en inicializar fecha y hora, pedir hora y agregar evento
    manager.estado = PEDIDO_INVALIDO;

    eventAdder_t adder; //Se usa en agregar evento
    adder.estado = ENVIANDO_MENSAJE_DE_COMAND;
    uint8_t entradaDeEventos[3];

    event_kicker_t kicker; //Se usa en quitar evento
    kicker.estado = ENVIANDO_INSTRUCCIONES;

    event_voicer_t voicer; // Se usa en consultar lista de eventos
    voicer.estado = ENVIANDO_FORMATO;
    voicer.contador = 0;

    char salida[10]; //Se usa en consultar lista de eventos

    struct tm tiempoParaAplicaciones; //Se usa en pedir y consultar hora para procesar tiempo

    //Inicializo estructuras
    inicilizarEventos();
    inicializarLedsRGB(ledsRGB);


    while (1) {
        //Maquina de estado UNO
        switch (estado.status) {
            case(EN_ESPERA): //Si la maquina de estados esta esperando un input
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
                        if (numBytes > 0) { //Esto significa que hubo un input
                            if (primeraVez) { //Si es la primera vez que se ingreso algo se inicializa fecha y hora
                                putrsUSBUSART("\r\nInicializacion\r\n");
                                estado.status = NO_INICIALIZADA; //Se pasa a este estado hasta que se inicialize
                                primeraVez = false; 
                            } else {
                                switch (buffer[0]) {
                                    case('m'): //Los estados indican que significa cada opcion
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
                                cleanBuffer(buffer); //Se limpia el buffer para evitar leer caractres viejos
                            }
                        }
                    }
                }
                break;
            case(NO_INICIALIZADA):
                if (inicializarFechaYHora(&init, &manager)) { //Cuando se haya inicializado
                    manager.estado = PEDIDO_INVALIDO; 
                    estado.status = EN_MENU; //Se muestra el menu
                }
                break;
            case(EN_MENU):
                if (enviarMensaje(MENU)) { //Cuando se muestre el menu se va a tiempo de espera
                    estado.status = EN_ESPERA;
                }
                break;
            case(EN_INGRESAR_HORA):
                if (primeraVezMensajes == true) { //La primera vez que se llame aqui imprime el formato de hora
                    if (enviarMensaje(FORMATO_DE_HORA)) {
                        primeraVezMensajes = false;
                    }
                }
                if (pedirHora(&tiempoParaAplicaciones, &manager)) { //Si se ingreso la hora
                    RTCC_TimeSet(&tiempoParaAplicaciones); //Se setea en el tiempo del RTC
                    primeraVezMensajes = true;
                    estado.status = EN_MENU; //De vuelta al menu
                }
                break;
            case(EN_MOSTRAR_HORA):
                if (consultarHora(&tiempoParaAplicaciones)) { //Cuando se haya mostrado la hora 
                    estado.status = EN_MENU; //De vuelta al menu
                }
                break;
            case(EN_AGREGAR_EVENTO):
                if (agregarEvento(&adder, entradaDeEventos, &manager)) {//Cuando se haya agregado el evento
                    adder.estado = ENVIANDO_MENSAJE_DE_COMAND; //Se resetea el adder
                    estado.status = EN_MENU; //Se vuelve al menu
                }
                break;
            case(EN_QUITAR_EVENTO):
                if (quitarEvento(&kicker, entradaDeEventos)) { //Si se quito el evento
                    kicker.estado = ENVIANDO_INSTRUCCIONES; //Se resetea el kicker
                    estado.status = EN_MENU; //Se vuelve al menu
                }
                break;
            case(EN_CONSULTAR_LISTA_DE_EVENTOS):
                if (consultarListaDeEventos(&voicer, salida)) { //Si termino la consulta
                    voicer.estado = ENVIANDO_FORMATO; //Se resetea el estado
                    voicer.contador = 0; //Se resetea el contador
                    estado.status = EN_MENU; //Se vuelve al menu
                }
                break;
            default:
                break;
        }
        //Maquina de estado dos
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

        for (i = 0; i < EVENTOS_MAXIMOS - 1; i++) { //Recorremos los eventos
            if (eventos[i].command != 0xFF) { //Si el evento es valido
                RTCC_TimeGet(&tiempoActual); //Obtenemos el tiempo actual
                uint32_t tiempoActualPlano = mktime(&tiempoActual); 
                if (tiempoActualPlano >= eventos[i].time) { //Si ya paso el horario de activacion de evento
                    if (eventos[i].command == 0) { //Si el evento es de apagar 
                        ledsRGB[eventos[i].param] = BLACK; //Se apaga el led
                    } else {
                        switch (eventos[i].color) { //Aca se averigua el color que tiene el evento como atributo
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
            } else {
                //Si el evento no esta configurado se apaga el led
                ledsRGB[eventos[i].param] = BLACK;
            }
        }
        WS2812_send(ledsRGB, 8); //Se prenden leds una vez terminado el seteo de colores
    }
}


