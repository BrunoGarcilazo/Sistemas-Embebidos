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
    uint32_t dispositivoID;
    uint8_t numeroDeContacto;
    uint8_t umbralDeTemperatura;
    bool midiendo;
    bool inicializado;
} termometro_t;

termometro_t dispositivo; 

void interfazPrincipal();

#endif /* _INTERFAZ_PRINCIPAL_H */

/* *****************************************************************************
 End of File
 */
