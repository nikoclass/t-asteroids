#ifndef COLISIONABLE_H_
#define COLISIONABLE_H_

#include "Condicionable.h"

class Vector;
class Objeto3d;
class Engine;
class MFisica;
class Esfera;
class ModoJuego;

class ModoJuego;
class Misil;
class MFisica;

class Colisionable : public Condicionable
{
private:
    Objeto3d* modelo;
    bool dead;

    bool remoto;

public:
    Colisionable(Objeto3d*modelo,Vector posicion);
    virtual ~Colisionable();

    Vector getPosicion();
    float getRadio();
    Esfera getEsfera();
    bool isDead()
    {
        return this->dead;
    }
    void setDead(bool dead);

    Objeto3d*getModelo()
    {
        return this->modelo;
    }
    void setModelo(Objeto3d*modelo);
    void posicionar(Vector pos);
    void escalar(float factor);
    void trasladar(Vector pos);

    void setRemoto();
    void setDeadRemoto();
    bool isRemoto(){return remoto;}

    virtual void tic(float dt)=0;
    virtual void bang(Colisionable*)=0;
};

#include "Vector.h"
#include "Objeto3d.h"
#include "Engine.h"
#include "MFisica.h"
#include "Esfera.h"
#include "ModoJuego.h"

#endif /*COLISIONABLE_H_*/
