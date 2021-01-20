#ifndef CLIENTENAVEPESADA_H
#define CLIENTENAVEPESADA_H

#include "ClienteRed.h"
#include "Nave.h"



class ClienteNavePesada : public ClienteRed
{
    public:
        ClienteNavePesada();
        virtual ~ClienteNavePesada();
        Nave* Getnave() { return nave; }
        void Setnave(Nave* val) { nave = val; }

        void recibirPaqueteCliente(PaqueteRed* paquete); //override de ClienteRed
        ClienteRed* clonar(); //Para los prototipos

        void onInit(); //override de ClienteRed
        void onDispose(); //override de ClienteRed

    private:
        Nave* nave;
};

#endif // CLIENTENAVEPESADA_H
