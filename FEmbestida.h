#ifndef FEMBESTIDA_H
#define FEMBESTIDA_H

#include "FMunicion.h"



class FEmbestida : public FMunicion
{
    public:
        FEmbestida(Nave* origen);
        virtual ~FEmbestida();
        void disparar(Vector posicion,Vector direccion);
        void tic(float deltate){};
        int getLado(){};
        void setLado(int){};
        void setConTarget(bool){};
        bool isConTarget(){};
};
#endif // FEMBESTIDA_H
