#include "ClienteMisil.h"

ClienteMisil::ClienteMisil():ClienteRed("Misil"){
    misil=NULL;
}

ClienteMisil::~ClienteMisil(){
}

void ClienteMisil::onInit(){
}

void ClienteMisil::onDispose(){
    if(misil)
        misil->setDeadRemoto();
TRAZA;
}

ClienteRed* ClienteMisil::clonar(){
    return new ClienteMisil();
}

void ClienteMisil::recibirPaqueteCliente(PaqueteRed* paquete){

    Vector pos=paquete->getVector();
    Vector dir=paquete->getVector();

    if(!misil){
        misil = new Misil(new Objeto3d("misil.mdl"),pos,dir);
        misil->setTtl(9999999);
        misil->setRemoto();
    }

    if(misil){
        Objeto3d *m=misil->getModelo();
        m->posicionar(pos);
        m->rotar(m->getEjeZ(),dir);
    }

}
