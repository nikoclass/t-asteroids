#ifndef EMBESTIDA_H
#define EMBESTIDA_H

#include "Colisionable.h"
#include "Vector.h"
#include "Sprite.h"
#include "Objeto3d.h"


class Embestida : public Colisionable
{
    public:
        Embestida(Objeto3d*obj, Vector pos);
        virtual ~Embestida();
        Objeto3d* Getfuego() { return fuego; }
        void Setfuego(Objeto3d* val) { fuego = val; }
        bool GetesActivo() { return esActivo; }
        void SetesActivo(bool val) { esActivo = val; }
        float GetTtl() { return Ttl; }
        void SetTtl(float val) { Ttl = val; }
        float Getdanio() { return danio; }
        void Setdanio(float val) { danio = val; }
        float Getmasa() { return masa; }
        void Setmasa(float val) { masa = val; }

    private:
        Objeto3d* fuego;
        bool esActivo;
        float Ttl;
        float danio;
        float masa;

        void tic(float);
        void bang(Colisionable*);
};

#endif // EMBESTIDA_H
