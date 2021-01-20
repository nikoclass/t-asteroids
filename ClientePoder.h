#ifndef CLIENTEPODER_H
#define CLIENTEPODER_H

#include "ClienteRed.h"
#include "Poder.h"


class ClientePoder : public ClienteRed
{
    public:
        ClientePoder();
        virtual ~ClientePoder();

        void recibirPaqueteCliente(PaqueteRed* paquete); //para override

        void onInit();
        void onDispose();

        ClienteRed* clonar();

    private:
        Poder* p;
};

#endif // CLIENTEPODER_H
