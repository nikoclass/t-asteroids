#include "ClienteBalaComun.h"

ClienteBalaComun::ClienteBalaComun():ClienteRed("BalaComun"){
}

ClienteBalaComun::~ClienteBalaComun(){
}

void ClienteBalaComun::onInit(){
    PaqueteRed* p=new PaqueteRed();
    //p->addInt(1);
    enviarPaqueteCliente(p);
    bala=NULL;
}

void ClienteBalaComun::onDispose(){
    if(bala)
        bala->setDeadRemoto();
}


void ClienteBalaComun::recibirPaqueteCliente(PaqueteRed* paquete){
    if(bala){

    }else{

    Objeto3d*modelo=new Objeto3d("balacomun.mdl");
	modelo->setAfectaLuces(false);
    modelo->setModoTransparencia(Aditivo);
    modelo->setOpacidad(0.7);

    Vector pos=paquete->getVector();
    Vector dir=paquete->getVector();
    int roja=paquete->getInt();

    modelo->rotar(Vector(0,0,-1),dir);

    bala=new BalaComun(modelo,pos,dir);
    bala->setTtl(999999);
    bala->setRemoto();


    if (roja)
        bala->setRoja();

    }
}

ClienteRed* ClienteBalaComun::clonar(){
    return new ClienteBalaComun();
}
