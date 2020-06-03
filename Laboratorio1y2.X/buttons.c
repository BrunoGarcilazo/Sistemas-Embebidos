#include <stdio.h>

#include <stdbool.h>
#include "buttons.h"

bool  Boton1Flag;
bool  Boton2Flag;

void Boton2Flag_Set( void ){
    Boton2Flag = true;
}

void Boton2Flag_Reset( void ) {
    Boton2Flag = false;
}

bool Boton2Flag_GetValue( void ){
    return Boton2Flag;
}

void Boton1Flag_Set( void ){
    Boton1Flag = true;
}

void Boton1Flag_Reset( void ){
    Boton1Flag = false;
}

bool Boton1Flag_GetValue( void ){
    return Boton1Flag;
}