#ifndef CLIENTENAVEPICHI_H
#define CLIENTENAVEPICHI_H
class ClienteNavePichi;

#include "ClienteRed.h"

class NavePichi;

class ClienteNavePichi : public ClienteRed{

    private:
        NavePichi *nave;

    public:
        ClienteNavePichi();
        virtual ~ClienteNavePichi();

        void recibirPaqueteCliente(PaqueteRed* paquete); //override de ClienteRed
        ClienteRed* clonar(); //Para los prototipos

        void onInit(); //override de ClienteRed
        void onDispose(); //override de ClienteRed

};

#include "NavePichi.h"

#endif // CLIENTENAVE_H
