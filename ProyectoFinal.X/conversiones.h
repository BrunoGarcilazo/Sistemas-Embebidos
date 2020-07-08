/* ************************************************************************** */
/** Descriptive File Name

  @Company
    UCUDAL

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _CONVERSIONES_H    /* Guard against multiple inclusion */
#define _CONVERSIONES_H




/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include <stdint.h>

typedef struct{
    uint8_t dispositivoID;
    uint8_t numeroDeContacto;
    uint8_t umbralDeTemperatura;
    bool midiendo;
} termometro_t;

uint16_t samplesConversiones[10];
extern const termometro_t dispositivo; 

void conversiones(void *p_params);

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
