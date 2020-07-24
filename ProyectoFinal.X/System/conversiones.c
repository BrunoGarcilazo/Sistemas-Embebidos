/** Conversiones

  @Company
    UCUDAL

  @File Name
    conversiones.c

  @Summary
    Se encarga de las mediciones de temperatura del dispositivo

  @Description
    Mide, promedia, convierte la medida a grados. Prende leds cuando esta midiendo y luego leds de colores 
    segun la salud del paciente
 */

// <editor-fold defaultstate="collapsed" desc="Includes">

//FreeRtos
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

//Libreria
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

//MCC
#include "../mcc_generated_files/usb/usb_device_cdc.h"
#include "../mcc_generated_files/pin_manager.h"
#include "../mcc_generated_files/adc1.h"

//Archivos del sistema
#include "conversiones.h"
#include "../Platform/ledManager.h"
#include "../Platform/gpsManager.h"
#include "../UI/interfazConversiones.h"
#include "../UI/interfazUSB.h"
#include "../Communications/GPS.h"
#include "../Communications/SIM808.h"
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Variables Globales">

xSemaphoreHandle prenderYapagarLucesRojas;
xSemaphoreHandle prenderYapagarLucesVerdes;

uint8_t contador = 0;

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Funciones de Interfaz">

/**
 * Promedia los valores de los samples
 * @param samples Conversiones Array con samples a promediar
 * @param tamanio Tamaño de array, cantidad de medidas
 * @return valor entre 0 - 1023
 */
float promedioSamples(float * samplesConversiones, uint8_t tamanio) {
    uint8_t i;
    float sumaDeValores = 0;
    for (i = 0; i < tamanio; i++) {
        sumaDeValores = sumaDeValores + samplesConversiones[i];
    }
    return sumaDeValores / tamanio;
}

/**
 * Convierte el Promedio de los datos del ADC a una Temperatura en Celsius entre 32.0C y 42.0C
 * @param promedio Promedio de las temperaturas medidas por el ADC
 * @return Valor en grados
 */
float conversorADCTemp(float promedio) {
    float temperatura;

    temperatura = ((promedio / ADC1_MAX) * TEMP_RANGO_MAX) + TEMP_MIN;
    return temperatura;
}

/**
 * Este metodo toma una medida y envia un mensaje al numero de contacto del dispositivo
 * El formato del mensaje es: id del dispositivo, temperatura, fecha y hora y link de google maps
 * @param medida Medida de la cual sacar los datos
 */
void alertarPersona(medida_t *medida) {
    uint8_t mensaje[100];
    uint8_t idDeDispositivo[10];
    uint8_t posicionStr[55];
    uint8_t fechaYhora[24];
    uint8_t temperaturaStr[5];
    struct tm *tiempoDeMedida;


    //Tomamos el id del dispositivo y lo pasamos a string
    sprintf(idDeDispositivo, "%d", dispositivo.dispositivoID);
    strcpy(mensaje, idDeDispositivo);
    strcat(mensaje, " ");

    //Convertimos la temperatura a string y la concatenamos al mensaje
    sprintf(temperaturaStr, "%.1f", medida->temperaturaRegistrada);
    strcat(mensaje, temperaturaStr);
    strcat(mensaje, " ");

    //Convertimos la fecha y hora a struct tm y despues a string
    tiempoDeMedida = gmtime(&medida->tiempo);
    strftime(fechaYhora, sizeof (fechaYhora), " %d/%m/%Y-%H:%M ", tiempoDeMedida);
    strcat(mensaje, fechaYhora);

    //Generamos el link de google maps
    GPS_generateGoogleMaps(posicionStr, medida->posicion);
    strcat(mensaje, posicionStr);

    //Enviamos el mensaje
    SIM808_sendSMS(dispositivo.numeroDeContacto, mensaje);
}

void conversiones(void *p_params) {
    uint8_t conversionesAHacer = 10;
    float samplesConversiones[conversionesAHacer];
    uint8_t contador = 0;
    float muestra;
    float promedio;

    GPSPosition_t posicion;
    medida_t medida;

    prenderYapagarLucesRojas = xSemaphoreCreateBinary();
    prenderYapagarLucesVerdes = xSemaphoreCreateBinary();


    //Tareas que checkean cada 100 ms si prender y apagar las luces del color determinado
    xTaskCreate(prenderLedsRojosPor2Seg, "LucesRojas", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 6, NULL);
    xTaskCreate(prenderLedsVerdesPor2Seg, "LucesVerdes", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 6, NULL);

    while (1) {
        /*Si existen los semaforos de tramaValida y de permiso de medicion de la interfaz*/
        if (tramaValida != NULL && puedoMedir != NULL) {

            /*Si los dos semaforos estan libres*/
            if (xSemaphoreTake(tramaValida, 0) == pdTRUE &&
                    xSemaphoreTake(puedoMedir, 0) == pdTRUE) {

                /*Liberamos el puedo medir, asi interfazConversiones sabe que puede detener la conversion*/
                xSemaphoreGive(puedoMedir);

                /*Mientras contador sea menor al numero de conversiones a hacer*/
                while (contador != conversionesAHacer) {

                    /*Hacemos take del semaforo de permiso de la interfaz por si se detuvo la medida*/
                    if (xSemaphoreTake(puedoMedir, 0) == pdTRUE) {
                        xSemaphoreGive(puedoMedir);
                        //taskENTER_CRITICAL();
                        ADC1_ChannelSelect(channel_AN15);
                        ADC1_SoftwareTriggerEnable();
                        vTaskDelay(pdMS_TO_TICKS(250));
                        ADC1_SoftwareTriggerDisable();
                        while (!ADC1_IsConversionComplete(channel_AN15)) {
                        }
                        muestra = ADC1_ConversionResultGet(channel_AN15);
                        //taskEXIT_CRITICAL();
                        samplesConversiones[contador] = muestra; //se guardan las 10 conversiones en un array
                        invertirLedsMedicion(); //se prenden/apagan leds azules por cada conversion
                        contador = contador + 1;
                    } else { /*Si se detuvo la medida apagamos los leds y cortamos el while*/
                        apagarLeds();
                        break;
                    }
                }
                contador = 0;

                /*Checkeamos el permiso*/
                if (xSemaphoreTake(puedoMedir, 0) == pdTRUE) {

                    /*Lo habilitamos para que pueda blockearse*/
                    xSemaphoreGive(puedoMedir);

                    /*Hacemos el promedio de las muestras*/
                    promedio = promedioSamples(samplesConversiones, conversionesAHacer);

                    /*Comvertimos el promedio de conversiones a temeperatura*/
                    promedio = conversorADCTemp(promedio);

                    /*Tomamos los datos de posicion de la trama*/
                    GPS_getPosition(&posicion, dispositivo.trama);

                    /*Tomamos el tiempo del sistema, este tiempo se actualiza cada 1 seg*/
                    medida.tiempo = mktime(&tiempoDelSistema);

                    /*Seteamos la posicion de GPS en la medida*/
                    medida.posicion = posicion;

                    /*Seteamos la temperatura*/
                    medida.temperaturaRegistrada = promedio;

                    if (ultimaMedida < MEDIDAS_MAX) {// Si el indice ultimaMedida esta dentro del array
                        /*Seteamos en el array*/
                        mediciones[ultimaMedida] = medida;
                        /*Incrementamos el indice a agregar en el array de medidas*/
                        ultimaMedida++;
                    }



                    /*Checkeamos el permiso*/
                    if (xSemaphoreTake(puedoMedir, 0) == pdTRUE) {
                        if (promedio > dispositivo.umbralDeTemperatura) { //Si la temperatura fue mayor al umbral
                            xSemaphoreGive(prenderYapagarLucesRojas); //Habilita a la tarea de luces rojas
                            alertarPersona(&medida); //Manda mensaje a la persona
                        } else {
                            xSemaphoreGive(prenderYapagarLucesVerdes); //Habilita a la tarea de luces verdes
                        }
                    } else { /*Si se blockeo la medida continuamos el bucle y apagamos las luces*/
                        continue;
                    }
                } else { /*Si la medida esta blockeada vamos a la siguiente iteracion del bucle general*/
                    apagarLeds();
                    continue;
                }
                /*Libera los semaforos de trama valida e inicializado*/
                xSemaphoreGive(tramaValida);
                xSemaphoreGive(inicializado);

                /*No se libera el permiso de interfaz porque este solo debe ser liberado desde interfazConversiones*/
            } else {
                vTaskDelay(pdMS_TO_TICKS(30)); //Demora 30 ms en pedir de nuevo permiso para medir
            }
        } else { //Si no existen los semaforos espera 1 seg
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}

// </editor-fold>

