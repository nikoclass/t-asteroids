#include "Municion.h"

Municion::Municion()
{
    //ctor
}

Municion::~Municion()
{
    //dtor
}

bool Municion::mismoOrigen(Nave *target){
    return this->origen == target;
}
