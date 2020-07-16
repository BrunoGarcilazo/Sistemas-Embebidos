/* ************************************************************************** */
/** Interfaz Conversiones

  @Company
    UCUDAL

  @File Name
    interfazConversiones.h

  @Summary
    Tareas que se encargan de lo relacionado con las medidas de temperatura del sistema

  @Description
    Controlar, guardar y tomar las conversiones analogico a digital del sistema
 */
/* ************************************************************************** */

#ifndef _INTERFAZ_CONVERSIONES_H    /* Guard against multiple inclusion */
#define _INTERFAZ_CONVERSIONES_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

typedef struct{
    uint32_t dispositivoID;      // ID unico del dispositivo
    uint8_t numeroDeContacto[12];    // Numero de contacto de forma 96123456
    uint8_t trama[65];          //Trama de GPS del dispositivo
    float umbralDeTemperatura; // Umbral de Temperatura. 32 < umbral < 42
    bool midiendo;               // variable que indica si se esta realizando una medicion (se utiliza si se presiona el boton mientras ya se esta midiendo)
    bool enUSB;                  // Indica si el dispositivo se encuentra en comunicacion USB
    bool inicializado;           // Indica si el Termometro ya cuenta con todos los datos necesarios para su correcto funcionamiento.   
} termometro_t;

termometro_t dispositivo;

bool boton2Flag;

void interfazTermometro(void* params);

#endif /* _INTERFAZ_CONVERIONES_H */

/* *****************************************************************************
 End of File
 */
