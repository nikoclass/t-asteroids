#include "ClienteRed.h"

ClienteRed::ClienteRed(string servidorAsociado){
    this->servidorAsociado=servidorAsociado;
    this->hashServidorAsociado=0;
}

void ClienteRed::setHashServidorAsociado(int hash){
    hashServidorAsociado=hash;
}

int ClienteRed::getHashServidorAsociado(){
    return hashServidorAsociado;
}

ClienteRed::~ClienteRed(){
}

void ClienteRed::enviarPaqueteCliente(PaqueteRed* paquete){
    ControladorRed::getInstancia()->enviarPaqueteCliente(this,paquete);
}

string ClienteRed::getServidorAsociado(){
    return servidorAsociado;
}
