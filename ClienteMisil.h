#ifndef CLIENTEMISIL_H
#define CLIENTEMISIL_H
class ClienteMisil;

#include "ClienteRed.h"
#include "Misil.h"

class ClienteMisil : public ClienteRed{

    private:
        Misil *misil;

    public:
        ClienteMisil();
        virtual ~ClienteMisil();

        void recibirPaqueteCliente(PaqueteRed* paquete); //para override

        void onInit();
        void onDispose();

        ClienteRed* clonar(); //Para los prototipos
};

#endif // CLIENTEMISIL_H
