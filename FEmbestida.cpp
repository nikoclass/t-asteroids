#include "FEmbestida.h"

FEmbestida::FEmbestida(Nave* origen):FMunicion(origen)
{
    this->cantMunicion = 10;
}

FEmbestida::~FEmbestida()
{
    //dtor
}

void FEmbestida::disparar(Vector posicion,Vector direccion){
        TRAZA;
        this->origen->setEmbistiendo();
}
