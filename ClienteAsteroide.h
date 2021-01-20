#ifndef CLIENTEASTEROIDE_H
#define CLIENTEASTEROIDE_H
class ClienteAsteroide;

#include "ClienteRed.h"
#include "Asteroide.h"


class ClienteAsteroide : public ClienteRed{

    private:
        Asteroide *asteroide;


    public:
        ClienteAsteroide();
        virtual ~ClienteAsteroide();

        void recibirPaqueteCliente(PaqueteRed* paquete); //para override

        void onInit();
        void onDispose();

        ClienteRed* clonar(); //Para los prototipos

};

#endif // CLIENTEASTEROIDE_H
