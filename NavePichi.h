#ifndef NAVEPICHI_H
#define NAVEPICHI_H
class NavePichi;

#include "Nave.h"
#include "ServidorRed.h"
#include "ClienteNavePichi.h"


class NavePichi : public Nave, public ServidorRed{
    public:
        NavePichi(Objeto3d*obj,Vector posicion);
        virtual ~NavePichi();
        void explotar();

        void recibirPaqueteServidor(PaqueteRed* paquete);
        string getNombreServidor();
        void disparar();


    private:
        Objeto3d*luz;

        void ticVivo(float dt);

};

#endif // NAVEPICHI_H
