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

/** Metodo llamado cuando se realiza una Medicion
 * Si los Leds estaban en Negro, se cambian a Azul y viceversa.
 */
void invertirLedsMedicion(){
    
    if(coloresLeds[0] == BLACK){
        for( i=0; i < coloresLeds.length; i++ ){
            coloresLeds[i] = BLUE;
        
        }        
    }else{
        for( i=0; i < coloresLeds.length; i++ ){
            coloresLeds[i] = BLACK;   
        }  
        
    }
    
    WS2812_send(coloresLeds, coloresLeds.length);   
}



/* *****************************************************************************
 End of File
 */
