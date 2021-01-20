#ifndef CLIENTEMODOJUEGO_H
#define CLIENTEMODOJUEGO_H
class ClienteModoJuego;

#include "ClienteRed.h"

class ClienteModoJuego : public ClienteRed
{
    private:

    public:
        ClienteModoJuego();
        virtual ~ClienteModoJuego();

        void recibirPaqueteCliente(PaqueteRed* paquete); //para override

        void onInit();
        void onDispose();

        ClienteRed* clonar(); //Para los prototipos

};

#endif // CLIENTEMODOJUEGO_H
