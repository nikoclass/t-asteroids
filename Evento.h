#ifndef EVENTO_H
#define EVENTO_H
class Evento;

#include <windows.h>
#include <iostream>
using namespace std;


// Clase para el manejo de señales de sincronizacion entre threads
class Evento{

    private:
        HANDLE evento;
        //bool ocupado;
        //bool esperando;

    public:
        Evento(bool signaled=false);
        virtual ~Evento();

        void wait(bool forzarReset=true);
        void signal();
};

#endif // EVENTO_H
