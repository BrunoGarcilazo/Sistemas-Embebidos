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

/* This section lists the other files that are included in this file.
 */

typedef struct{
    uint8_t dispositivoID;
    uint8_t numeroDeContacto;
    uint8_t umbralDeTemperatura;
    bool midiendo;
} termometro_t;

extern const termometro_t dispositivo; 

void interfazPrincipal();

#endif /* _INTERFAZ_PRINCIPAL_H */

/* *****************************************************************************
 End of File
 */
