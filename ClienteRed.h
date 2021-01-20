#ifndef CLIENTERED_H
#define CLIENTERED_H
class ClienteRed;

#include "PaqueteRed.h"
#include "Hasheable.h"

#include <iostream>
using namespace std;

//Objeto cliente de red, sirve para atender los paquetes que le envian los objetos
//servidores de red de la maquina host.
//Es abstracto por lo que cada subclase debe implementar los metodos recibirPaquete()
//y clonar(). Este ultimo debe devolver una instancia de la misma clase derivada
//Esto es usado para que los prototipos devuelvan objetos clientes nuevos para atender
//a los objetos servidores nuevos.
//En su constructor debe recibir el nombre del objeto servidor asociado.
//La clase que herede, debe registrarse en los archivos PrototiposClientes.h y PrototiposClientes.cpp

class ClienteRed: public Hasheable{

    private:
        string servidorAsociado;
        int hashServidorAsociado;

    public:
        ClienteRed(string servidorAsociado);
        virtual ~ClienteRed();

        void enviarPaqueteCliente(PaqueteRed* paquete);

        virtual void recibirPaqueteCliente(PaqueteRed* paquete)=0; //para override

        virtual void onInit(){}
        virtual void onDispose(){}

        virtual ClienteRed* clonar()=0; //Para los prototipos

        void setHashServidorAsociado(int hash);
        int getHashServidorAsociado();

        string getServidorAsociado();
};

#include "ControladorRed.h"

#endif // CLIENTERED_H
