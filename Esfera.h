#ifndef ESFERA_H
#define ESFERA_H

#include "Matematicas.h"

class Esfera
{
    public:
        Esfera();
        Esfera(Vector centro,float radio);

        Vector getCentro();
        float getRadio();

        bool intersecta(Esfera &otra);

    private:
        Vector centro;
        float radio;
};

#endif // ESFERA_H
