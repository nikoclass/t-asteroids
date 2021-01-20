#ifndef LENSFLARE_H
#define LENSFLARE_H

#include "Luz.h"
#include "Dibujable.h"
#include <map>

#define CANT_FLARES 7

class Sprite;

class LensFlare{
private:

    Luz* luz;
    Escena* escena;

    Sprite* flares[CANT_FLARES];
    map<Camara*,float> factoresDeTapado;

public:
    LensFlare(Luz* luz);
    virtual ~LensFlare();


    void toGL();




};

#include "Sprite.h"

#endif // LENSFLARE_H

