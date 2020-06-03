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

/* This section lists the other files that are included in this file.
 */
#include "events.h"
#include "../app.h"
#include "../../framework/LEDs_RGB/LEDs_RGB_fwk.h"


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */
/** Descriptive Data Item Name

  @Summary
    Brief one-line summary of the data item.
    
  @Description
    Full description, explaining the purpose and usage of data item.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
  @Remarks
    Any additional remarks
 */
app_event_t eventos[EVENT_MAX];


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */
/** 
  @Function
    int ExampleLocalFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Description
    Full description, explaining the purpose and usage of the function.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

  @Precondition
    List and describe any required preconditions. If there are no preconditions,
    enter "None."

  @Parameters
    @param param1 Describe the first parameter to the function.
    
    @param param2 Describe the second parameter to the function.

  @Returns
    List (if feasible) and describe the return values of the function.
    <ul>
      <li>1   Indicates an error occurred
      <li>0   Indicates an error did not occur
    </ul>

  @Remarks
    Describe any special behavior not described above.
    <p>
    Any additional remarks.

  @Example
    @code
    if(ExampleFunctionName(1, 2) == 0)
    {
        return 3;
    }
 */
static void EVT_execute( app_event_t p_event ){
    if(p_event.param==EVENT_PARAM_ON){
        RGB_setLedColor( p_event.param, p_event.color );
    }else if(p_event.param==EVENT_PARAM_OFF){
        RGB_setLedColor( p_event.param, RGB_BLACK );
    }
    RGB_showLeds( LED_RGB_SIZE );
}


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

// *****************************************************************************

/** 
  @Function
    int ExampleInterfaceFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Remarks
    Refer to the example_file.h interface header for function usage details.
 */
void EVT_checkAndExecute( void ){
    uint8_t i;

    for(i=0; i<EVENT_MAX; i++){
        if((eventos[i].command!=EVENT_PARAM_NOT_CONFIG) && (eventos[i].time>=APP_getTimestamp( ))){
            EVT_execute( eventos[i] );
            eventos[i].command=EVENT_PARAM_NOT_CONFIG; // Borro el evento
        }
    }
}

int8_t EVT_isAvailable( void ){
    uint8_t i;

    for(i=0; i<EVENT_MAX; i++){
        if(eventos[i].command==EVENT_PARAM_NOT_CONFIG){
            return i;
        }
    }
    return -1;
}

bool EVT_add( app_event_t p_event ){
    uint8_t i;

    for(i=0; i<EVENT_MAX; i++){
        if(eventos[i].command==EVENT_PARAM_NOT_CONFIG){
            eventos[i]=p_event;
            return true;
        }
    }
    return false;
}


/* *****************************************************************************
 End of File
 */
