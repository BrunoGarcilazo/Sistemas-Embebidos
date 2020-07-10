/* ************************************************************************** */
/**

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _INTERFAZ_PRINCIPAL_H    /* Guard against multiple inclusion */
#define _INTERFAZ_PRINCIPAL_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include <stdbool.h>
#include <stdint.h>

typedef struct{
    uint32_t dispositivoID; // ID unico del dispositivo
    uint8_t numeroDeContacto;   // Numero de contacto de forma 96123456
    uint8_t umbralDeTemperatura; // Umbral de Temperatura. 32 < umbral < 42
    bool midiendo;               // variable que indica si se esta realizando una medicion (se utiliza si se presiona el boton mientras ya se esta midiendo)
    bool inicializado;           // Indica si el Termometro ya cuenta con todos los datos necesarios para su correcto funcionamiento.   
} termometro_t;

termometro_t dispositivo; 


void interfazPrincipal();

#endif /* _INTERFAZ_PRINCIPAL_H */

/* *****************************************************************************
 End of File
 */
