/**
 Funciones para el manejo de nuestra Placa
 
 */

/*Libraries*/
#include <time.h>
#include <stdbool.h>
#include <string.h>

/*MCC Includes*/
#include "../mcc_generated_files/rtcc.h"

#include "rtcManager.h"
#include "usbManager.h"
#include "deviceManager.h"
#include "../System/menu.h"
#include "../System/scheludeManager.h"

uint8_t id_Dispositivo;

/** Le pido al Usuario un ID para identificar el Dispositivo
 */
void obtenerID(uint8_t* id){
    
    enviarMensaje(FORMATO_DE_ID);
                           
    buscarEntrada(id, sizeof(id));
    
    id_Dispositivo = id;
    
}
