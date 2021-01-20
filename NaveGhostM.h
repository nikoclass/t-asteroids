#ifndef NAVEGHOSTM_H
#define NAVEGHOSTM_H
class NaveGhostM;

#include "Nave.h"
#include "Electricidad.h"


class NaveGhostM : public Nave, public ServidorRed
{
    public:
        NaveGhostM(Objeto3d*obj,Vector posicion);
        virtual ~NaveGhostM();
        void explotar();
        void disparar();
        void recibirPaqueteServidor(PaqueteRed* paquete);
        string getNombreServidor();
        void setTargetElectricidad(Targeteable *t){targetElectricidad=t;}

    private:
        Objeto3d*fuegoD;
        Objeto3d*fuegoI;
        Sprite *brilloFuegoI;
        Sprite *brilloFuegoD;
        Electricidad *electricidad;
        void ticVivo(float dt);

        Targeteable *targetElectricidad;
};

#endif // NAVEGHOSTM_H
