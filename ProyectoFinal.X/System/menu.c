/** Menu
  @Company
    UCUDAL

  @File Name
    Menu.c

  @Summary
    Contiene la funcion menu y las funciones que ingresan y muestran datos por 
    consola

  @Description
    Funciones asociadas al menu que leen y escriben datos via USB
 */

// <editor-fold defaultstate="collapsed" desc="Include Files">
//Includes de FreeRtos
#include "FreeRTOS.h"
#include "task.h"

//Librerias
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

//MCC
#include "../mcc_generated_files/usb/usb_device_cdc.h"

//Archivos del programa
#include "menu.h"
#include "../UI/interfazConversiones.h"
#include "../System/conversiones.h"
#include "../Platform/usbManager.h"
#include "../Platform/gpsManager.h"
#include "../UI/interfazUSB.h"
#include "../System/utils.h"
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Funciones de Interfaz">

/**
 * Recibe un string como parametro y valida si su formato corresponde a un numero de telefono celular
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
 * Funcion que verifica si una temperatura dada por el Usuario es valida o no
 * @param entrada dada por el Usuario
 * @return entrada valida o no
 */
bool validarTemperatura(char *entrada) {
    uint8_t parteEntera;
    parteEntera = ((10 * (entrada[0] - ASCII_TO_INT_DIFFERENCE)) + (entrada[1] - ASCII_TO_INT_DIFFERENCE));
    if (parteEntera > 42 || parteEntera < 32) {
        return false; //Si la parte entera no esta entre los valores medibles es falso
    } else {
        if (entrada[2] != '.') { //Si no se ingreso un punto en el medio es falso
            return false;
        } else {
            /*Si la parte entera es 42 el unico valor posible decimal es 0*/
            if (parteEntera == 42 && (entrada[3] - ASCII_TO_INT_DIFFERENCE) != 0) {
                return false;
                /*Sino checkeamos si la parte decimal esta en valores posibles*/
            } else if ((entrada[3] - ASCII_TO_INT_DIFFERENCE) < 0 || (entrada[3] - ASCII_TO_INT_DIFFERENCE) > 9) {
                return false;
            } else {
                /*Si no pasa ninguna de las anteriores lo ingresado es valido*/
                return true;
            }
        }
    }
}

/**
 *  Funcion que pide el Umbral de Temperatura al Usuario hasta que ingrese uno valido
 */
void pedirUmbralTemperatura() {
    bool verificado;
    uint8_t entrada[5];
    do {
        enviarMensaje(PEDIDO_DE_UMBRAL); //Envio mensaje que pide el ingreso
        memset(entrada, 0, sizeof (entrada)); //Limpiamos la entrada
        buscarEntrada(entrada, sizeof (entrada)); //Recibo entrada
        if (strlen(entrada) > 4) {//Si me enviaron algo con largo mayor al requerido vuelvo a pedir
            continue;
        }
        verificado = validarTemperatura(entrada); //Verifico la validez
        if (verificado) { //Si es valido seteo entrada y salgo del bucle
            dispositivo.umbralDeTemperatura = atof(entrada);
            break;
        }
    } while (!verificado); //Todo esto mientras no sea valido

    //Al final aviso que el dato enviado fue correcto
    enviarMensaje(DATO_CORRECTO);
}

/**
 * Pide al Usuario un ID para el Dispositivo hasta que mande uno valido
 */
void pedirID() { // sin terminar
    uint32_t idAingresar;
    uint8_t largoDeEntrada = 11;
    uint32_t idMaximo = 4294967295;
    uint8_t i;
    char entrada[largoDeEntrada]; // Array donde se va a recibir la entrada
    bool entradaValida = true; //Flag para saber cuando cortar el bucle

    do {
        enviarMensaje(PEDIDO_DE_ID);
        memset(entrada, 0, sizeof (entrada)); //Se limpia la entrada
        buscarEntrada(entrada, sizeof (entrada)); //Pedimos entrada
        if (strlen(entrada) > 10) { //Si el largo de la entrada es mayor al requerido volvemos a iterar
            continue;
        }
        for (i = 0; i < strlen(entrada); i++) { // Metodo de conversion de Array de char a int : shorturl.at/acoAL
            if ((entrada[i] - ASCII_TO_INT_DIFFERENCE) >= 10 || (entrada[i] - ASCII_TO_INT_DIFFERENCE) < 0) { // Solo puede ser numerico
                entradaValida = false; //Se setea a false para checkear mas abajo
            }
            idAingresar = 10 * idAingresar + (entrada[i] - ASCII_TO_INT_DIFFERENCE);
        }
        if (!entradaValida || idAingresar > idMaximo || idAingresar < 0) { //Condiciones de invalidez
            entradaValida = true; //Se setea a true para que siga el bucle
            continue;
        } else {//Si la entrada es valida seteamos y cortamos el bucle
            dispositivo.dispositivoID = idAingresar;
            break;
        }
    } while (entradaValida);

    //Avisamos que los datos estan correctos
    enviarMensaje(DATO_CORRECTO);
}

/**
 * Funcion que pide un numero de contacto al Usuario hasta que ingrese uno valido
 */
void pedirNumeroDeContacto() {
    uint8_t i, k;
    uint8_t largoDeNumeroDeCelular = 9;
    char entrada[largoDeNumeroDeCelular + 1];
    uint8_t numeroFormatoSMS[largoDeNumeroDeCelular + 2]; //Array donde vamos a agregar las comillas
    do {
        enviarMensaje(FORMATO_DE_TELEFONO); // Enviamos formato
        memset(entrada, 0, (sizeof (entrada))); //Se limpia la entrada
        buscarEntrada(entrada, (sizeof (entrada))); //Pedimos entrada

        if (strlen(entrada) > largoDeNumeroDeCelular) { //Si el largo de lo ingresado es mayor a lo requerido
            continue; //Volvemos a pedir
        }
        if (validarTelefono(entrada)) {
            // Convierto la entrada en un array de char con el formato \"092020400\"
            numeroFormatoSMS[0] = '"';
            for (i = 0; i<sizeof (entrada); i++) {
                numeroFormatoSMS[i + 1] = entrada[i];
            }
            numeroFormatoSMS[largoDeNumeroDeCelular + 1] = '"';
            // Ahora numeroFormatoSMS = "096123456"
            for (k = 0; k < sizeof (entrada); k++) {
                dispositivo.numeroDeContacto[k] = numeroFormatoSMS[k];
            }
            break;
        }
    } while (1);

    //Avisamos que se setearon los datos bien
    enviarMensaje(DATO_CORRECTO);
}

/**
 * Funcion que envia por USB las medidas guardadas hasta el momento
 */
void imprimirMedidas() {

    /*Creamos los templates iniciales*/
    char id[15];
    strcpy(id, "\r\nMedida ");

    char temperaturaMensaje[26];
    strcpy(temperaturaMensaje, "\r\nTemperatura: ");

    char fechaMensaje[40];
    strcpy(fechaMensaje, "\r\nFecha y Hora: ");

    char posicionMensaje[80];
    strcpy(posicionMensaje, "\r\nLocalizacion: ");

    /*Creamos arrays auxiliares para los datos*/
    char indiceStringID[3];
    char posicionString[55];
    char temperatura[4];
    uint8_t fechaYhora[24];

    //Algunas variables auxiliares
    uint8_t i;
    struct tm *tiempoMedida;
    medida_t *medida;
    time_t timetMedida;
    
    for (i = 0; i < ultimaMedida; i++) {
        //Apuntamos a la medida requerida
        medida = &mediciones[i];

        /*Usamos el indice dentro del array para ID de medida. Lo pasamos a string y concatenamos al template*/
        sprintf(indiceStringID, "%d", i);
        strcat(id, indiceStringID);

        /*Tomamos el tiempo de la medida y lo pasamos a struct tm, luego a string y luego al template*/
        timetMedida = mediciones[i].tiempo;
        tiempoMedida = gmtime(&timetMedida);
        strftime(fechaYhora, 24, " %d/%m/%Y-%H:%M ", tiempoMedida);
        strcat(fechaMensaje, fechaYhora);

        /*Convertimos la temperatura de la medida a string y la ponemos en el template*/
        ftoa(medida->temperaturaRegistrada, temperatura, 1);
        strcat(temperaturaMensaje, temperatura);

        /*Creamos el link de goolge maps con la ubicacion de la medida ESTO FALLA*/
        GPS_generateGoogleMaps(posicionString, mediciones[i].posicion);
        strcat(posicionMensaje, posicionString);
        strcat(posicionMensaje, "\r\n");

        /*Enviamos todos los templates*/
        enviarMensaje(id);
        enviarMensaje(fechaMensaje);
        enviarMensaje(temperaturaMensaje);
        enviarMensaje(posicionMensaje);

        //Colocamos todas las variables en estados "inciciales" otra vez
        memset(temperatura, 0, sizeof temperatura);
        memset(fechaYhora, 0, sizeof fechaYhora);
        memset(fechaMensaje, 0, sizeof fechaMensaje);
        memset(posicionMensaje, 0, sizeof posicionMensaje);
        memset(temperaturaMensaje, 0, sizeof temperaturaMensaje);

        strcpy(id, "\r\nMedida ");
        strcpy(temperaturaMensaje, "\r\nTemperatura: ");
        strcpy(fechaMensaje, "\r\nFecha y Hora: ");
        strcpy(posicionMensaje, "\r\nLocalizacion: ");
        //Enviamos cada un segundo
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void menu() {
    MENU_STATUS status;
    uint8_t buffer[4]; //Buffer donde se recibe la primer entrada
    uint8_t numBytes; //Numero de bytes recibidos
    status = EN_MENU;

    /**
     * Lista de menu:
     * Poner ID de dispositivo
     * Umbral de temperatura
     * Telefono para enviar mensajes
     * Imprimir lista de medidas
     * Borrar todos las medidas
     * Terminar conexion
     */
    while (buffer[0] != 'f') { //Mientras la entrada no sea 
        if (USBUSARTIsTxTrfReady()) {
            switch (status) {
                case(EN_MENU): //Si el estado es de menu imprimimos el mismo
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
                                pedirID();
                                status = EN_MENU;
                                break;
                            case ('b'): //Umbral de temperatura
                                pedirUmbralTemperatura();
                                status = EN_MENU;
                                break;
                            case ('c')://Telefono para enviar mensajes
                                pedirNumeroDeContacto();
                                status = EN_MENU;
                                break;
                            case ('d')://Imprimir lista de medidas 
                                imprimirMedidas();
                                status = EN_MENU;
                                break;
                            case ('e')://Borrar medidias 
                                status = EN_MENU;
                                break;
                            case ('f')://Terminar conexion
                                enviarMensaje("\r\nConexion terminada. Gracias por confiar\r\n");
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
// </editor-fold>
