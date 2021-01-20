#ifndef CONTROLADORCONEXIONRED_H
#define CONTROLADORCONEXIONRED_H
class ControladorConexionRed;
class ProcesadorPaquetes;

#include "Thread.h"
#include "Socket.h"
#include "ControladorRed.h"
#include "ObserversConexion.h"

//Esta clase es la encargada de correr un thread en paralelo para atender
//los pedidos de conexion en caso de servidor, o atender la lluvia de paquete en caso del cliente
//y tomar las medidas necesarias como por ejemplo, crear un Controlador de Cliente para el cliente
//o redirigir los paquetes que vienen del servidor a los objetos clientes.
class ControladorConexionRed : public SubjectConexion, public Thread{

    private:

        SocketServer* in;
        SocketClient* out;

        ModoControlador modo;
        int puerto;
        string direccion;

    public:
        ControladorConexionRed();
        virtual ~ControladorConexionRed();

        void comenzarServidor(int puerto);
        void comenzarCliente(string direccion,int puerto);

        void matarServidor();
        void matarCliente();

        void run(); //override para el thread

        void enviarPaqueteAServidor(PaqueteRed* paquete);
};


#include "ProductorConsumidor.h"

//Thread encargado de procesar los paquetes que recibe del ControladorConexionRed
class ProcesadorPaquetes: public Thread{
    private:
        ProductorConsumidor *productor_consumidor;

    public:
        ProcesadorPaquetes();
        ~ProcesadorPaquetes();

        void run();

        //Ingresa al objeto productor_consumidor el paquete dado para que el thread lo atienda cuando pueda
        void procesarPaquete(PaqueteRed *paquete);
};

#endif // CONTROLADORCONEXIONRED_H
