#include "ClientePoder.h"


ClientePoder::ClientePoder(): ClienteRed("Poder"){
    this->p= NULL;
}

ClientePoder::~ClientePoder(){
}

void ClientePoder::recibirPaqueteCliente(PaqueteRed* paquete){ //para override
    Vector posicion = paquete->getVector();
    int pow = paquete->getInt();

    if(!this->p){

        this->p = new Poder(new Objeto3d("esfera.mdl"),Vector());
        this->p->setRemoto();

        this->p->SetPoder(pow);
        this->p->posicionar(posicion);
    }
}
void ClientePoder::onInit(){
    enviarPaqueteCliente(new PaqueteRed());
}
void ClientePoder::onDispose(){
        if(p)
            p->setDeadRemoto();
}

ClienteRed* ClientePoder::clonar(){
    return new ClientePoder();
}
