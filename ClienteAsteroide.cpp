#include "ClienteAsteroide.h"

#include "Logger.h"

ClienteAsteroide::ClienteAsteroide():ClienteRed("Asteroide"){
    asteroide=NULL;
}

ClienteAsteroide::~ClienteAsteroide(){
}

void ClienteAsteroide::onInit(){
    enviarPaqueteCliente(new PaqueteRed());
}

void ClienteAsteroide::onDispose(){
    if(asteroide)
        asteroide->setDeadRemoto();
}

ClienteRed* ClienteAsteroide::clonar(){ //Para los prototipos
    return new ClienteAsteroide();
}

void ClienteAsteroide::recibirPaqueteCliente(PaqueteRed* paquete){ //para override

    Vector posicion=paquete->getVector();
    Vector velocidad=paquete->getVector();
    Vector velocidadAngular=paquete->getVector();
    float nivel=paquete->getFloat();
    string modelo=paquete->getString();
    string textura=paquete->getString();

    if(!asteroide){
        Objeto3d *o=new Objeto3d(modelo.data(),textura.data());
        asteroide=new Asteroide(1,nivel,o,posicion);
        asteroide->setRemoto();
    }

    asteroide->posicionar(posicion);
    asteroide->setVelocidad(velocidad);
    asteroide->setVelocidadAngular(velocidadAngular);

}

