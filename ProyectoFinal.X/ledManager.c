/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include "Platform/WS2812.h"
#include "ledManager.h"


/** Metodo llamado cuando se realiza una Medicion
 * Si los Leds estaban en Negro, se cambian a Azul y viceversa.
 */
void invertirLedsMedicion(){
    uint8_t i;
    if(coloresLeds[0].b == 0){
        for( i = 0; i < 8; i++ ){ //PREGUNTAR PORQUE NO ANDA SIZEOF
            coloresLeds[i] = BLUE;
        
        }        
    }else{
        for( i=0; i < 8; i++ ){
            coloresLeds[i] = BLACK;   
        }  
        
    }
    WS2812_send(coloresLeds,8);   
}



/* *****************************************************************************
 End of File
 */
