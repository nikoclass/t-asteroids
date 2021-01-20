#ifndef CLIENTENAVEGHOST_H
#define CLIENTENAVEGHOST_H

#include "ClienteRed.h"
#include "Nave.h"


class ClienteNaveGhost : public ClienteRed
{
    public:
        ClienteNaveGhost();
        virtual ~ClienteNaveGhost();
        void recibirPaqueteCliente(PaqueteRed* paquete); //para override

        virtual void onInit();
        virtual void onDispose();

        virtual ClienteRed* clonar(); //Para los prototipos

    private:
        Nave* nave;
};

#endif // CLIENTENAVEGHOST_H
