#include "ClienteModoJuego.h"
#include "ModoJuego.h"

#include "NavePichi.h"
#include "NavePesada.h"
#include "NaveGhostM.h"

ClienteModoJuego::ClienteModoJuego():ClienteRed("ModoJuego"){
}

ClienteModoJuego::~ClienteModoJuego(){
}

void ClienteModoJuego::onInit(){
    PaqueteRed *p=new PaqueteRed();

    p->addString("Cliente Nuevo");
    p->addInt(ControladorRed::getInstancia()->getNumCliente());

    Nave *n=Settings::getInstance()->getNave();


    if(dynamic_cast<NavePichi*>(n))
        p->addString("JFighter");
    else if(dynamic_cast<NaveGhostM*>(n))
        p->addString("Ghost");
    else if(dynamic_cast<NavePesada*>(n))
        p->addString("Pesada");
    else //por defecto
        p->addString("JFighter");


    enviarPaqueteCliente(p);

    ModoJuego::getInstance()->setCliente(this);

}

void ClienteModoJuego::onDispose(){
}


ClienteRed* ClienteModoJuego::clonar(){ //Para los prototipos
    return new ClienteModoJuego();
}

void ClienteModoJuego::recibirPaqueteCliente(PaqueteRed* paquete){ //para override

    Engine::getInstancia()->setTimeScale(paquete->getFloat());
    ModoJuego::getInstance()->setPausa(paquete->getInt());
}

