#ifndef CLIENTECHAT_H
#define CLIENTECHAT_H

#include "ClienteRed.h"


class ClienteChat : public ClienteRed
{

    private:
    public:
        ClienteChat();
        virtual ~ClienteChat();

        void recibirPaqueteCliente(PaqueteRed* paquete); //para override

        void onInit();
        void onDispose();

        ClienteRed* clonar(); //Para los prototipos

        static ClienteChat *instancia;

};

#endif // CLIENTECHAT_H
