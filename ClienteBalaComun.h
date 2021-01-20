#ifndef CLIENTEBALACOMUN_H
#define CLIENTEBALACOMUN_H
class ClienteBalaComun;

//#include "ControladorRed.h"
#include "ClienteRed.h"
#include "BalaComun.h"

class ClienteBalaComun: public ClienteRed{

    private:
        BalaComun* bala;


    public:
        ClienteBalaComun();
        virtual ~ClienteBalaComun();

        void recibirPaqueteCliente(PaqueteRed* paquete); //override de ClienteRed
        ClienteRed* clonar(); //Para los prototipos

        void onInit(); //override de ClienteRed
        void onDispose(); //override de ClienteRed
};

#endif // CLIENTEBALACOMUN_H
