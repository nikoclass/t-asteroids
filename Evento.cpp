#include "Evento.h"

Evento::Evento(bool signaled){

    evento = CreateEvent(
        NULL,               // atributos de seguridad
        TRUE,               // manual-reset
        signaled,           // estado inicial
        NULL                // sin nombre
        );

    if(!evento){
        cout<<"ERROR: Error al crear objeto Evento (no deberia suceder nunca)"<<endl;
        exit(1); //aborto todo
    }

    //esperando=false;
}

Evento::~Evento(){
    CloseHandle(evento);
}

void Evento::wait(bool forzarReset){

    //esperando=true;

    DWORD resultado = WaitForSingleObject(
        evento,     // handle
        INFINITE);  // espera infinita

    //esperando=false;

    if(resultado!=WAIT_OBJECT_0){
        cout<<"ERROR: Error al realizar wait sobre un objeto Evento (no deberia suceder nunca)"<<endl;
        exit(1);
    }

    if(forzarReset)
        if(!ResetEvent(evento)){
            cout<<"ERROR: Error al realizar wait sobre un objeto Evento (no deberia suceder nunca)"<<endl;
            exit(1);
        }

}

void Evento::signal(){
    //if(!esperando) return;

    if (!SetEvent(evento)){
        cout<<"ERROR: Error al realizar signal sobre un objeto Evento (no deberia suceder nunca)"<<endl;
        exit(1);
    }

}
