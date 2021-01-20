#ifndef MUNICION_H
#define MUNICION_H

class Nave;

class Municion
{
    public:
        Municion();
        virtual ~Municion();
        Nave* Getorigen() { return origen; }
        void Setorigen(Nave* val) { origen = val; }
        bool mismoOrigen(Nave *target);

    private:
        Nave* origen;
        //int origen; //id del cliente de red ahora
};

#include "Nave.h"

#endif // MUNICION_H
