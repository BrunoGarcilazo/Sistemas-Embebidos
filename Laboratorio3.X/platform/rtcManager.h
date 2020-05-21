/* ************************************************************************** */
/** En este archivo hay funciones que manejan el RTC en base a interaccion via USB

  @Company
    UCUDAL

  @File Name
    rtcManager.h

  @Summary
    Funciones que usan el RTC y USB

  @Description
    Procesar input del usuario e interactuar con el USB
 */
/* ************************************************************************** */

#ifndef _RTC_MANAGER_H    /* Guard against multiple inclusion */
#define _RTC_MANAGER_H

/* ************************************************************************** */
/* Section: Include                                                           */
/* ************************************************************************** */
#include <time.h>

/* ************************************************************************** */
/* Section: Funciones de interfaz                                             */
/* ************************************************************************** */

    /**
     * Le pide hora al operador via USB y la coloca en el RTC
     * El formato de hora debe ser Hora-Minutos-Segundos (sin caracteres en medio)
     */
    struct tm pedirHora(void);
    
    /**
     * Imprime en la consola el tiempo actual via USB
     */
    void consultarHora(void);
    
#endif /* _RTC_MANAGER_H */

/* *****************************************************************************
 End of File
 */
