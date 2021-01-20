#ifndef FELECTRICIDAD_H
#define FELECTRICIDAD_H
class FElectricidad;

#include "FMunicion.h"
class Electricidad;


class FElectricidad : public FMunicion {
    private:
        int lado;
        float ttl;
        int danio;
        bool conTarget;
        float dt;

     public:
        //Constructores
        FElectricidad(Nave* Origen);
        virtual ~FElectricidad();

        //Funciones
        void disparar(Vector posicion,Vector direccion);
        void tic(float deltate);

        bool isConTarget();
        void setConTarget(bool in);
        int getLado();
        void setLado(int lado);
        int getDanio();
        void setDanio(int danio);

};

#endif // FELECTRICIDAD_H
#include "Electricidad.h"
