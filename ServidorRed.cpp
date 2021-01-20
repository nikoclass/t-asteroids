#include "ServidorRed.h"

#include "Logger.h"

int ServidorRed::uid=0;

ServidorRed::ServidorRed(){
    this->id=++uid;
    ControladorRed::getInstancia()->registrarServidor(this);
}

ServidorRed::~ServidorRed(){
    ControladorRed::getInstancia()->desregistrarServidor(this);
}

/*
string ServidorRed::getNombreServidor(){
    string s=string(typeid(*this).name());
    int l=s.size();
    if(l<=10)
        return s.substr(1,250);
    else
        return s.substr(2,250);
}
*/

void ServidorRed::enviarPaqueteServidor(PaqueteRed* paquete){
    ControladorRed::getInstancia()->enviarPaqueteServidor(this,paquete);
}

int ServidorRed::getHashCode(){
    return id;
}
