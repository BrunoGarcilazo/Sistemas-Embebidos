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

#include "UI.h"
#include "../../framework/USB/USB_fwk.h"
#include <string.h>
#include <ctype.h>


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
const uint8_t ui_welcomeText[] = "Bienvenido al calendario de eventos de Sistemas Embebidos\nPor favor presione una tecla para continuar...\n";
const uint8_t ui_optionsText[] = "Indique la opción deseada:\n1) Setear hora del sistema\n2)Consultar hora del sistema\n3) Consultar eventos activos\n4) Control de eventos\n";

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
void UI_showMenu( void ) {
    static ui_menu_states_t menuState = UI_MENU_STATE_INIT_SHOW;
    static uint8_t rxData[24];

    if( USB_getConnectedStatus() == true ) {
        switch( menuState ) {
            case( UI_MENU_STATE_INIT_SHOW):
                if( USB_send((uint8_t*) ui_welcomeText) == true ) {
                    memset(rxData, 0, sizeof (rxData));
                    menuState = UI_MENU_STATE_INIT_WAIT;
                }
                break;
            case( UI_MENU_STATE_INIT_WAIT):
                if( UI_waitForInput(rxData) == true ) {
                    menuState = UI_MENU_STATE_OPTIONS_SHOW;
                }
                break;
            case( UI_MENU_STATE_OPTIONS_SHOW):
                if( USB_send((uint8_t*) ui_optionsText) == true ) {
                    memset(rxData, 0, sizeof (rxData));
                    menuState = UI_MENU_STATE_OPTIONS_WAIT;
                }
                break;
            case( UI_MENU_STATE_OPTIONS_WAIT):
                if( UI_waitForInput(rxData) == true ) {
                    menuState = UI_MENU_STATE_OPTIONS_CHECK;
                }
                break;
            case( UI_MENU_STATE_OPTIONS_CHECK):
                if( (UI_checkValidOption(rxData, UI_OPTION_NUM, 4, 1)) == true ) {
                    menuState = UI_MENU_STATE_OPTIONS_CHECK + atoi(rxData);
                }
                else {
                    menuState = UI_MENU_STATE_OPTIONS_SHOW;
                }
                break;
            case( UI_MENU_STATE_SET_TIMEDATE):
                //@ToDo: UI_setTimedate()
                break;
            case( UI_MENU_STATE_GET_TIMEDATE):
                //@ToDo: UI_showTimedate()
                break;
            case( UI_MENU_STATE_GET_EVENTS):
                //@ToDo: UI_showEvents()
                break;
            case( UI_MENU_STATE_SET_EVENTS):
                //@ToDo: UI_setEventsMenu()
                //          -> Setear evento
                //          -> Borrar evento
                break;
        }
    }
    else {
        menuState = UI_MENU_STATE_INIT_SHOW;
    }
}

bool UI_waitForInput( uint8_t *p_dest ) {
    uint8_t bytesReceived;

    bytesReceived = USB_receive(p_dest);
    if( bytesReceived > 0 ) {
        p_dest[bytesReceived] = '\0';
        return true;
    }
    return false;
}

bool UI_checkValidOption( uint8_t *p_src, ui_options_t p_type, uint32_t p_max, uint32_t p_min ) {
    uint32_t intValue;
    uint32_t i;

    switch( p_type ) {
        case UI_OPTION_NUM:
            for( i = 0; i < strlen(p_src); i++ ) {
                if( isdigit(p_src[i]) == 0 ) {
                    return false;
                }
            }
            intValue = atoi(p_src);
            if( (intValue < p_min) || (intValue > p_max) ) {
                return false;
            }
            break;

        case UI_OPTION_ALPHANUM:
            for( i = 0; i < strlen(p_src); i++ ) {
                if( isalnum(p_src[i]) == 0 ) {
                    return false;
                }
            }
            if( strlen(p_src) > p_max ) {
                return false;
            }
            break;

        case UI_OPTION_ALPHA:
            for( i = 0; i < strlen(p_src); i++ ) {
                if( isalpha(p_src[i]) == 0 ) {
                    return false;
                }
            }
            if( strlen(p_src) > p_max ) {
                return false;
            }
            break;
    }
    return true;
}


/* *****************************************************************************
 End of File
 */
