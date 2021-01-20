#ifndef SERVIDORRED_H
#define SERVIDORRED_H
class ServidorRed;


class PaqueteRed;

#include "Hasheable.h"

#include <iostream>
using namespace std;



//Objeto servidor de red, envia y recibe paquetes hacia el controlador de red,
//luego el controlador de red lo manda a todos los clientes registrados, los cuales los hacen
//llegar a todos los objetos clientes
//Es abstracto y para heredarlo hay que sobreescribir el metodo recibiPaquete(...)
class ServidorRed: public Hasheable{

    private:

        static int uid;
        int id;

    public:

        ServidorRed();
        virtual ~ServidorRed();

        void enviarPaqueteServidor(PaqueteRed* paquete);

        virtual void recibirPaqueteServidor(PaqueteRed* paquete)=0;

        virtual string getNombreServidor()=0;

        int getHashCode(); //Del Hasheable
};


#include "ControladorRed.h"
#include "PaqueteRed.h"

#endif // SERVIDORRED_H
