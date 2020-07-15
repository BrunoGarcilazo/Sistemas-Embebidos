/* ************************************************************************** */
/**InterfazUSB

  @Company
    UCUDAL

  @File Name
    interfazUSB.c

  @Summary
    Gestion de la interaccion del mundo exterior con la placa 

  @Description
    Crear tareas para manejar la interaccion de la placa con el mundo
 */
/* ************************************************************************** */

#ifndef _interfazUSB_H    /* Guard against multiple inclusion */
#define _interfazUSB_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include <stdbool.h>
#include <stdint.h>

#define ASCII_TO_INT_DIFFERENCE 48

void interfazUSB(void* params);

void mantenimientoUSB(void * p_param);

typedef struct{
    uint32_t dispositivoID;      // ID unico del dispositivo
    uint8_t numeroDeContacto[11];    // Numero de contacto de forma 96123456
    float umbralDeTemperatura; // Umbral de Temperatura. 32 < umbral < 42
    bool midiendo;               // variable que indica si se esta realizando una medicion (se utiliza si se presiona el boton mientras ya se esta midiendo)
    bool inicializado;           // Indica si el Termometro ya cuenta con todos los datos necesarios para su correcto funcionamiento.   
} termometro_t;

void inicalizarUSB(void * p_param);

bool enUSB;

#endif /* _INTERFAZ_USB_H */

/* *****************************************************************************
 End of File
 */
