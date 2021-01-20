#ifndef CONTROLADORRED_H
#define CONTROLADORRED_H
class ControladorRed;
class TipoSnapshot;

enum ModoControlador{Inactivo=0,Servidor,Cliente};

#include "Engine.h"

#include "Socket.h"
#include "Thread.h"
#include "ServidorRed.h"
#include "ClienteRed.h"
#include "ControladorClienteRed.h"
#include "ControladorConexionRed.h"
#include "PaqueteRed.h"
#include "HashTable.h"
#include "ObserversConexion.h"

#include "SeccionCritica.h"

#include <iostream>
#include <list>
#include <map>
using namespace std;

//Clase principal de red, centraliza todo el envio y recepcion de paquetes de red
//para delegarlos correctamente a los objetos servidores y clientes respectivamente registrados
//Tiene 3 modos de funcionamiento:
//1) Inactivo; no hace nada con los paquetes que se quieren enviar ni atiende conexiones
//2) Servidor; envia a todos los clientes conectados los paquetes que se quieren enviar,
//atiende conexiones de clientes en determinado puerto
//y recibe los paquetes de los respectivos clientes para redirigirlos a los objetos servidores
//3) Cliente; Conecta con un servidor especificado, recibe y envia paquetes del servidor redirigiendo a los objetos clientes

class ControladorRed
    : public ObserverConexion, public SubjectConexion
{
    private:
        ControladorRed();
        static ControladorRed* instancia;

        ModoControlador modoControlador;

        //Servidor
        //-----------------------------------

        //hash con los objetos servidores registrados
        HashTable *objetosServidores;

        //lista con los clientes actualmente conectados
        list<ControladorClienteRed*> *controladoresClientes;

        //thread que escucha en el puerto especificado
        ControladorConexionRed* ctrlConexion;

        list<PaqueteRed*> *paquetesPendientes;
        list<TipoSnapshot*> *snapshotsPendientes;

        int puertoEscucha;

        int ultimoNumCliente;

        //Cliente
        //----------------------------------
        //hash con los objetos clientes registrados (con clave su servidor asociado)
        HashTable *objetosCliente;

        //map con los prototipos de clientes para clonar segun su tipo de servidor
        map<string,ClienteRed*> *prototiposCliente;

        string direccionServidor;

        int numCliente;

        SeccionCritica *mutex;
        //SeccionCritica *mutexSuspender;

    public:

        static ControladorRed* getInstancia();
        virtual ~ControladorRed();

        void setModoServidor(int puertoEscucha=2212);
        void setModoCliente(string servidor,int puerto=2212);
        void setModoInactivo();

        ModoControlador getModoControlador();

        void registrarServidor(ServidorRed *servidor);
        void desregistrarServidor(ServidorRed *servidor);

        void registrarPrototipo(ClienteRed *cliente);
        void desregistrarCliente(ClienteRed *cliente);

        void registrarControladorClienteRed(ControladorClienteRed* ctrlCliente);

        void enviarPaqueteServidor(ServidorRed *servidor, PaqueteRed *paquete);
        void enviarPaqueteCliente(ClienteRed *cliente, PaqueteRed *paquete);

        void enviarTodosLosPaquetes();

        void enviarSnapshot(ControladorClienteRed* ctrlCliente);
        //void enviarSnapshotPendiente(ControladorClienteRed* ctrlCliente);
        void recibirSnapshot(PaqueteRed *snapshot);

        //Ambas funciones reciben el hash del objeto servidor, no es un error
        void recibirPaquetesDeCliente(int hashServidor,PaqueteRed* paquete);
        void recibirPaquetesDeServidor(int hashServidor,PaqueteRed* paquete);

        int getUltimoNumCliente();

        int getNumCliente();
        void setNumCliente(int num);

        void suspender();
        void reanudar();

        virtual void onError(std::string message);
        virtual void onOK(std::string message);

        // Sobreescribe porque la misma clase es un ObserverConexion
        virtual void addObserver(ObserverConexion*);

};


//Clase para manejar los posibles snapshots que se den
class TipoSnapshot: public Hasheable{
    public:
        ServidorRed* servidor;
        int id;
        bool alta;

        TipoSnapshot(ServidorRed* servidor,bool alta);
};

#endif // CONTROLADORRED_H
