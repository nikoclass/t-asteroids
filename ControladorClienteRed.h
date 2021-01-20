#ifndef CONTROLADORCLIENTERED_H
#define CONTROLADORCLIENTERED_H
class ControladorClienteRed;

#include "Hasheable.h"
#include "Thread.h"
#include "Socket.h"
#include "ControladorRed.h"
#include "SeccionCritica.h"


//Esta clase se encarga de mantener la conexion con los clientes fisicos de red
//recibir asincronicamente los paquetes que ellos envian y delegarlos
//a los objetos servidores registrados en el controlador de red
//Tambien envia (sincronicamente por ahora) los paquetes que les da el controlador de red
//para que le lleguen a los objetos cliente de red
class ControladorClienteRed: public Hasheable, public Thread{

    private:
        int numCliente;
        Socket *cliente;
        SeccionCritica *mutex;
        bool activo;

    public:
        ControladorClienteRed(int numCliente,Socket* socket);
        virtual ~ControladorClienteRed();

        void run();

        int getHashCode(); //devuelve el numero de cliente

        void enviarPaqueteCliente(PaqueteRed* paquete);

        bool isActivo();

        void mutexEntrar();
        void mutexSalir();


};

#endif // CONTROLADORCLIENTERED_H
